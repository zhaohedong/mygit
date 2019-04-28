#include <iostream>
#include <string>
#include <list>
#include "include/str_list.h"
#include "common/ConfUtils.h"
#include "common/entity_name.h"
#include "common/code_environment.h"
#include "common/Mutex.h" 
#include "log/SubsystemMap.h" 
#include "common/config_obs.h"
#include "common/options.h"
#include "common/config.h"


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <boost/scoped_ptr.hpp>

#include <iostream>
#include <string>
using namespace std;

#include "osd/OSD.h"
#include "os/ObjectStore.h"
#include "mon/MonClient.h"
#include "include/ceph_features.h"

#include "common/config.h"

#include "mon/MonMap.h"

#include "msg/Messenger.h"

#include "common/Timer.h"
#include "common/TracepointProvider.h"
#include "common/ceph_argparse.h"

#include "global/global_init.h"
#include "global/signal_handler.h"

#include "include/color.h"
#include "common/errno.h"
#include "common/pick_address.h"

#include "perfglue/heap_profiler.h"

#include "include/assert.h"


#include "common/config.h"
#include "common/errno.h"
#include "gtest/gtest.h"
#include "common/hostname.h"

extern std::string exec(const char* cmd); // defined in test_hostname.cc

class test_md_config_t : public md_config_t, public ::testing::Test {
public:

  test_md_config_t()
    : md_config_t(true), Test()
  {}

/*    bool my_expand_meta(std::string &val,std::ostream *oss)
    {
        bool (*func)(std::string &val,std::ostream *oss) = &(md_config_t::expand_meta);
        func(val, oss);
    }
*/
  void test_expand_meta() {
    Mutex::Locker l(lock);
    // successfull meta expansion $run_dir and ${run_dir}
    {
      ostringstream oss;
      std::string before = " BEFORE ";
      std::string after = " AFTER ";

      std::string val(before + "$run_dir${run_dir}" + after);
      EXPECT_TRUE(expand_meta(val, &oss));
      EXPECT_EQ(before + "/var/run/ceph/var/run/ceph" + after, val);
      EXPECT_EQ("", oss.str());
    }
    {
      ostringstream oss;
      std::string before = " BEFORE ";
      std::string after = " AFTER ";
      std::string val(before + "$host${host}" + after);
      EXPECT_TRUE(expand_meta(val, &oss));
      std::string hostname = ceph_get_short_hostname();
      EXPECT_EQ(before + hostname + hostname + after, val);
      EXPECT_EQ("", oss.str());
    }
    // no meta expansion if variables are unknown
    {
      ostringstream oss;
      std::string expected = "expect $foo and ${bar} to not expand";
      std::string val = expected;
      EXPECT_FALSE(expand_meta(val, &oss));
      EXPECT_EQ(expected, val);
      EXPECT_EQ("", oss.str());
    }
    // recursive variable expansion
    {
      std::string host = "localhost";
      EXPECT_EQ(0, set_val("host", host.c_str(), false));

      std::string mon_host = "$cluster_network";
      EXPECT_EQ(0, set_val("mon_host", mon_host.c_str(), false));

      std::string lockdep = "true";
      EXPECT_EQ(0, set_val("lockdep", lockdep.c_str(), false));

      std::string cluster_network = "$public_network $public_network $lockdep $host";
      EXPECT_EQ(0, set_val("cluster_network", cluster_network.c_str(), false));

      std::string public_network = "NETWORK";
      EXPECT_EQ(0, set_val("public_network", public_network.c_str(), false));

      ostringstream oss;
      std::string val = "$mon_host";
      EXPECT_TRUE(expand_meta(val, &oss));
      EXPECT_EQ(public_network + " " +
                public_network + " " +
                lockdep + " " +
                "localhost", val);
      EXPECT_EQ("", oss.str());
    }
    // variable expansion loops are non fatal
    {
      std::string mon_host = "$cluster_network";
      EXPECT_EQ(0, set_val("mon_host", mon_host.c_str(), false));

      std::string cluster_network = "$public_network";
      EXPECT_EQ(0, set_val("cluster_network", cluster_network.c_str(), false));

      std::string public_network = "$mon_host";
      EXPECT_EQ(0, set_val("public_network", public_network.c_str(), false));

      ostringstream oss;
      std::string val = "$mon_host";
      EXPECT_TRUE(expand_meta(val, &oss));
      EXPECT_EQ("$cluster_network", val);
      const char *expected_oss =
        "variable expansion loop at mon_host=$cluster_network\n"
        "expansion stack: \n"
        "public_network=$mon_host\n"
        "cluster_network=$public_network\n"
        "mon_host=$cluster_network\n";
      EXPECT_EQ(expected_oss, oss.str());
    }
  }

  void test_expand_all_meta() {
    Mutex::Locker l(lock);
    int before_count = 0, data_dir = 0;
    for (const auto &i : schema) {
      const Option &opt = i.second;
      if (opt.type == Option::TYPE_STR) {
        const auto &str = boost::get<std::string>(opt.value);
        if (str.find("$") != string::npos)
          before_count++;
        if (str.find("$data_dir") != string::npos)
          data_dir++;
      }
    }

    // if there are no meta variables in the default configuration,
    // something must be done to check the expected side effect
    // of expand_all_meta
    ASSERT_LT(0, before_count);
    expand_all_meta();
    int after_count = 0;
    for (auto& i : values) {
      const auto &opt = schema.at(i.first);
      if (opt.type == Option::TYPE_STR) {
        const std::string &str = boost::get<std::string>(i.second);
        size_t pos = 0;
        while ((pos = str.find("$", pos)) != string::npos) {
          if (str.substr(pos, 8) != "$channel") {
            std::cout << "unexpected meta-variable found at pos " << pos
                      << " of '" << str << "'" << std::endl;
            after_count++;
          }
          pos++;
        }
      }
    }
    ASSERT_EQ(data_dir, after_count);
  }
};

TEST_F(test_md_config_t, expand_meta)
{
  test_expand_meta();
}

TEST_F(test_md_config_t, expand_all_meta)
{
  test_expand_all_meta();
}

TEST(md_config_t, set_val)
{
  int buf_size = 1024;
  md_config_t conf;
  // disable meta variable expansion
  {
    char *buf = (char*)malloc(buf_size);
    std::string expected = "$host";
    EXPECT_EQ(0, conf.set_val("mon_host", expected.c_str(), false));
    EXPECT_EQ(0, conf.get_val("mon_host", &buf, buf_size));
    EXPECT_EQ(expected, buf);
    free(buf);
  }
  // meta variable expansion is enabled by default
  {
    char *run_dir = (char*)malloc(buf_size);
    EXPECT_EQ(0, conf.get_val("run_dir", &run_dir, buf_size));
    EXPECT_EQ(0, conf.set_val("admin_socket", "$run_dir"));
    char *admin_socket = (char*)malloc(buf_size);
    EXPECT_EQ(0, conf.get_val("admin_socket", &admin_socket, buf_size));
    EXPECT_EQ(std::string(run_dir), std::string(admin_socket));
    free(run_dir);
    free(admin_socket);
  }
}

TEST(Option, validation)
{
  Option opt_int("foo", Option::TYPE_INT, Option::LEVEL_BASIC);
  opt_int.set_min_max(5, 10);

  std::string msg;
  EXPECT_EQ(-EINVAL, opt_int.validate(Option::value_t(int64_t(4)), &msg));
  EXPECT_EQ(-EINVAL, opt_int.validate(Option::value_t(int64_t(11)), &msg));
  EXPECT_EQ(0, opt_int.validate(Option::value_t(int64_t(7)), &msg));

  Option opt_enum("foo", Option::TYPE_STR, Option::LEVEL_BASIC);
  opt_enum.set_enum_allowed({"red", "blue"});
  EXPECT_EQ(0, opt_enum.validate(Option::value_t(std::string("red")), &msg));
  EXPECT_EQ(0, opt_enum.validate(Option::value_t(std::string("blue")), &msg));
  EXPECT_EQ(-EINVAL, opt_enum.validate(Option::value_t(std::string("green")), &msg));

  Option opt_validator("foo", Option::TYPE_INT, Option::LEVEL_BASIC);
  opt_validator.set_validator([](std::string *value, std::string *error_message){
      if (*value == std::string("one")) {
        *value = "1";
        return 0;
      } else if (*value == std::string("666")) {
        return -EINVAL;
      } else {
        return 0;
      }
  });

  std::string input = "666";  // An explicitly forbidden value
  EXPECT_EQ(-EINVAL, opt_validator.pre_validate(&input, &msg));
  EXPECT_EQ(input, "666");

  input = "123";  // A permitted value with no special behaviour
  EXPECT_EQ(0, opt_validator.pre_validate(&input, &msg));
  EXPECT_EQ(input, "123");

  input = "one";  // A value that has a magic conversion
  EXPECT_EQ(0, opt_validator.pre_validate(&input, &msg));
  EXPECT_EQ(input, "1");
}

/*
 * Local Variables:
 * compile-command: "cd ../.. ;
 *   make unittest_config &&
 *    valgrind \
 *    --max-stackframe=20000000 --tool=memcheck \
 *   ./unittest_config # --gtest_filter=md_config_t.set_val
 * "
 * End:
 */


using namespace std;
std::deque<std::string> parse_errors;
std::map<std::string, const Option &> schema;

//const std::vector<Option> ceph_options = build_options();

int main(int argc, const char **argv)
{
	const char *conf_files = "ceph.conf";
	vector<const char*> args;
	argv_to_vec(argc, argv, args);
	env_to_vec(args);

	vector<const char*> def_args;
	auto cct = global_init(&def_args, args, CEPH_ENTITY_TYPE_OSD,CODE_ENVIRONMENT_DAEMON,0, "osd_data");
//	md_config_t conf;
	test_md_config_t conf;
  	string fn = "ceph.conf";
  	std::ostream *warnings = &cerr;
//  	conf.my_expand_meta(fn, warnings);
	
//	ceph_heap_profiler_init();
/*
  	std::list<std::string> cfl;
  	get_str_list(conf_files, cfl);
	auto p = cfl.begin();
  while (p != cfl.end()) {
    // expand $data_dir?
    string &s = *p;
    cout << s << endl;
    p++;
      } 
*/

/*
    Option op1 = Option("host", Option::TYPE_STR, Option::LEVEL_BASIC)
    .set_description("local hostname")
    .set_long_description("if blank, ceph assumes the short hostname (hostname -s)")
    .add_service("common")
    .add_tag("network");

    Option op2 = Option("fsid", Option::TYPE_UUID, Option::LEVEL_BASIC)
    .set_description("cluster fsid (uuid)")
    .add_service("common")
    .add_tag("service");

   schema.insert(op1.name, &op1);
   schema.insert(op2.name, &op2);

  ConfFile cf;

  cf.clear();
  string fn = "ceph.conf";
  std::ostream *warnings = &cerr;
  conf.expand_meta(fn, warnings);
  int ret = cf.parse_file(fn.c_str(), &parse_errors, warnings);

  std::vector <std::string> my_sections;
  conf._get_my_sections(my_sections);
  for (const auto &i : schema) {
    const auto &opt = i.second;
    std::string val;
    int ret = conf._get_val_from_conf_file(my_sections, opt.name, val, false);
    if (ret == 0) {
      std::string error_message;
      int r = conf.set_val_impl(val, opt, &error_message);
      if (warnings != nullptr && (r != 0 || !error_message.empty())) {
        *warnings << "parse error setting '" << opt.name << "' to '" << val
                  << "'";
        if (!error_message.empty()) {
          *warnings << " (" << error_message << ")";
        }
        *warnings << std::endl;
      }
   }
  }
 */
  return 0;
}
