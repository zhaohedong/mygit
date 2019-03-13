#!/usr/bin/python
# -*- coding:utf-8 -*-


from selenium import webdriver #selenium 需要自己安装此模块
import time
 
driver= webdriver.Firefox()
driver.get("http://www.baidu.com")
driver.find_element_by_link_text((driver.find_element_by_name("tj_login").text).split()[5]).click()
time.sleep(1)
driver.find_element_by_id("user_name").clear()
time.sleep(1)
driver.find_element_by_id("TANGRAM__PSP_8__userName").send_keys("百度用户名")
time.sleep(1)
driver.find_element_by_id("TANGRAM__PSP_8__password").send_keys("百度密码")
time.sleep(3)
driver.find_element_by_id("TANGRAM__PSP_8__submit").click()
time.sleep(3)
driver.find_element_by_link_text(driver.find_element_by_class_name("user-name").text).click()
time.sleep(3)
driver.quit()
