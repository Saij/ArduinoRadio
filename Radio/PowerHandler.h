#ifndef __POWER_HANDLER_H__
#define __POWER_HANDLER_H__

class PowerHandler {
  public:
    static void setupPower();

  private:
    PowerHandler() {};

    bool _powerState = true;
};

#endif // __POWER_HANDLER_H__
