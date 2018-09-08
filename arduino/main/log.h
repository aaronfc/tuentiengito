#ifndef log_h
#define log_h

class Log {
  private:
    bool enabled;
  public:
    Log();
    void Log::enable();
    Log& Log::d(const char str[]);
    Log& Log::d(const int value);
    Log& Log::d(const unsigned int value);
    Log& Log::d(const long value);
    Log& Log::d(const unsigned long value);
    Log& Log::eol();
};

#endif