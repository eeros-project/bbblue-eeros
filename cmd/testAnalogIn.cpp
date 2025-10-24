#include <signal.h>

#include <eeros/logger/Logger.hpp>
#include <eeros/hal/HAL.hpp>
#include <eeros/core/Executor.hpp>
#include <eeros/control/PeripheralInput.hpp>
#include <eeros/control/TimeDomain.hpp>
#include <eeros/task/Periodic.hpp>
#include <eeros/task/Lambda.hpp>
#include <string>

using Logger = eeros::logger::Logger;
using Executor = eeros::Executor;
using HAL = eeros::hal::HAL;
using In = eeros::control::PeripheralInput<double>;
using Task = eeros::task::Periodic;
using Lambda = eeros::task::Lambda;
using TD = eeros::control::TimeDomain;

void signalHandler(int signum){
    Executor::stop();
}

int main(int argc, char **argv) {

  Logger::setDefaultStreamLogger(std::cout);
  Logger log = Logger::getLogger();

  log.info() << "Start Analog Input Test";

  log.info() << "Initializing hardware";
  HAL& hal = HAL::instance();
  hal.readConfigFromFile(&argc, argv);

  In dcJack("dc_jack");
  In battery("battery");
  std::string imu("acc");
  In ax(imu + "X");
  In ay(imu + "Y");
  In az(imu + "Z");
  In bar("bar");

  TD td("time domain", 0.1, false);
  td.addBlock(dcJack);
  td.addBlock(battery);
  td.addBlock(ax);
  td.addBlock(ay);
  td.addBlock(az);
  td.addBlock(bar);

  Lambda readOut([&](){
      log.info() << "DC: " << dcJack.getOut().getSignal().getValue()
      << " Bat: " << battery.getOut().getSignal().getValue() << ", "
      << imu << ' ' << ax.getOut().getSignal().getValue() << ", " << ay.getOut().getSignal().getValue() << ", " << az.getOut().getSignal().getValue()
      << " Bar: " << bar.getOut().getSignal().getValue();
  });
  Task readOutTask("read out", 1, readOut);
  auto& exec = Executor::instance();
  exec.setExecutorPeriod(0.1);
  exec.add(td);
  exec.add(readOutTask);
  signal(SIGINT, signalHandler);
  exec.run();
}
