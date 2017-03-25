#ifndef CTIME_H
#define CTIME_H

#include <string.h>
#include <sys/time.h>

struct CTimeData {
  time_t     t;
  struct tm *tm;
};

class CTime {
 public:
  CTime() {
    data_ = new CTimeData();

    data_->t = time(NULL);

    init();
  }

  CTime(time_t t) {
    data_ = new CTimeData();

    data_->t = t;

    init();
  }

 ~CTime() {
    delete data_;
  }

  int getYear    () { initTm(); return data_->tm->tm_year + 1900; }
  int getMonth   () { initTm(); return data_->tm->tm_mon ; }
  int getWeekDay () { initTm(); return data_->tm->tm_wday; }
  int getMonthDay() { initTm(); return data_->tm->tm_mday; }
  int getYearDay () { initTm(); return data_->tm->tm_yday; }
  int getHour    () { initTm(); return data_->tm->tm_hour; }
  int getMin     () { initTm(); return data_->tm->tm_min ; }
  int getSec     () { initTm(); return data_->tm->tm_sec ; }

  double diff(const CTime &time) {
    return difftime(data_->t, time.data_->t);
  }

  double diff(time_t t) {
    return difftime(data_->t, t);
  }

  std::string format(const char *format) {
    static char str[256];

    initTm();

    strftime(str, 256, format, data_->tm);

    return str;
  }

  std::string getLsTime(bool show_secs=false) {
    std::string str;

    CTime current_time;

    if (current_time.getYear() == getYear()) {
      if (current_time.getYearDay() - getYearDay() < 183) {
        if (! show_secs)
          str = format("%h %e %H:%M");
        else
          str = format("%h %e %H:%M:%S");
      }
      else
        str = format("%h %e  %Y");
    }
    else if (current_time.getYear() == getYear() + 1) {
      if (current_time.getYearDay() + (365 - getYearDay()) < 183) {
        if (! show_secs)
          str = format("%h %e %H:%M");
        else
          str = format("%h %e %H:%M:%S");
      }
      else
        str = format("%h %e  %Y");
    }
    else
      str = format("%h %e  %Y");

    return str;
  }

 private:
  void init() { data_->tm = NULL; }

  void initTm() {
    if (data_->tm == NULL) {
      data_->tm = new struct tm;

      memcpy(data_->tm, localtime(&data_->t), sizeof(struct tm));
    }
  }

 private:
  CTimeData *data_;
};

#endif
