#ifndef BBBLUE_EEROS_DEVICE_HPP_
#define BBBLUE_EEROS_DEVICE_HPP_

namespace bbblue {
	class BBBlueDevice {
	public:
		BBBlueDevice();
		virtual ~BBBlueDevice();
		
	private:
		static BBBlueDevice* instance;
	};
};

#endif /* BBBLUE_EEROS_DEVICE_HPP_ */
