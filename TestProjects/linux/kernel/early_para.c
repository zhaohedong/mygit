struct obs_kernel_param {
const char *str;
int (*setup_func)(char *);
int early;
};

/*
 *  * Only for really core code.  See moduleparam.h for the normal way.
 *   *
 *    * Force the alignment so the compiler doesn't space elements of the
 *     * obs_kernel_param "array" too far apart in .init.setup.
 *      */
#define __setup_param(str, unique_id, fn, early)			\
		static const char __setup_str_##unique_id[] __initconst	\
		__aligned(1) = str; \
	static struct obs_kernel_param __setup_##unique_id	\
		__used __section(.init.setup)			\
		__attribute__((aligned((sizeof(long)))))	\
		= { __setup_str_##unique_id, fn, early }

#define __setup(str, fn)					\
		__setup_param(str, fn, fn, 0)

/* NOTE: fn is as per module_param, not __setup!  Emits warning if fn
 *  * returns non-zero. */
#define early_param(str, fn)					\
		__setup_param(str, fn, fn, 1)

early_param("zhaohedong", yuanqi);

void main(void)
{
	printf("hello world!\r\n");
}
