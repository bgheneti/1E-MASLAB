ifndef INCLUDE_ENCODER_FIRMWARE_
define INCLUDE_ENCODER_FIRMWARE_
namespace firmware {
    class Encoder {
        private:
            mraa::Gpio gpio[2]; // the pins the encoder is attached to
            int state; // the state the encoder is currently in (0-3).
            int stateTicks[4]; // keeps track of the number of times each state is read
        public:
            Encoder(int inputPin1, int inputPin2);
            void poll();
            int getNumTicks();
            void resetNumTicks();
    };


}
#endif // INCLUDE_ENCODER_FIRMWARE_
