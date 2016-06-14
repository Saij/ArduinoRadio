#ifndef __POWER_HANDLER_H__
#define __POWER_HANDLER_H__

class PowerHandler {
  	public:
    	static void setup();

  	private:
    	bool _powerState = true;
};

#endif // __POWER_HANDLER_H__