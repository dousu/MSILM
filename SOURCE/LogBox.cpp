/*
 * LogBox.cpp
 *
 *  Created on: 2012/12/18
 *      Author: Hiroki Sudo
 */

#include "LogBox.h"

/* statics */

std::vector<std::string> LogBox::log;
std::string LogBox::log_file("");
int LogBox::log_size = 500;
int LogBox::threads = 1;

/*Constructor & destructor*/
LogBox::LogBox() {

}

LogBox::~LogBox() {
  refresh_log();
}

void
LogBox::push_log(std::string msg) {
  if (log_size > log.size()) {
    log.push_back(msg);
  }
  else {
    refresh_log();
    log.push_back(msg);
  }
}

void
LogBox::pop_log(int nth) {
  if (log.size() < nth) {
    std::string str;
    str = "**** CANCELED UPPER MESSAGE ****";
    log.push_back(str);
    return;
  }
  else {
    for (int i = 0; i < nth; i++)
      log.pop_back();
  }
}

void
LogBox::set_filepath(std::string file) {
  log_file = file;
}

void
LogBox::refresh_log(void) {
  if (log.size() > 0) {
    std::ofstream ofs(log_file.c_str(), std::ios::app);
    for (int i = 0; i < log.size(); i++) {
      ofs << log[i] << std::endl;
    }
    log.clear();
    std::vector<std::string>(log).swap(log);
  }
}