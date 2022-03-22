"""
Module Name: Hotspot Config
Authors: Dishant
Description: Hotspot Config service is responsible for providing the
    user a chance to reconfigure the controller. Reconfiguration means
    giving a new controllerId to the controller.

    This service keeps a watch on the hardware configuration button. If
    the button is pressed, it creates a hotspot network, lets user connect
    to the hotspot and access a webpage to enter the new configuration
    details. The controller restarts, resets itself and is configured
    according to the new controllerId.

    The webpage lets user enter controllerId and the network interface
    details. Network interface plays an critical role as the controller
    iot services will interact with the network through this
    interface. The network interface can either be lan or wifi.

    Also, This service functions only in those controllers which are
    compatible to run hotspot. This is confirmed by the mode
    environment variable. If the env variable exists and is not empty,
    it means it is compatible to run hotspot.
"""

import threading
import time
import RPi.GPIO
from services.service import Service
import logging

logger = logging.getLogger("__main__.hotspot_config")
logger.setLevel(logging.INFO)


class HotspotConfig(Service):
    """
    The start method will spawn a thread responsible for detection of controller
    configuration hardware button press.

    Attributes:
        resource (dict): References to all resources - database
            proxy, config interface, os interface, firehose logger,
            mqtt client.
        configInterface (:obj:ConfigInterface)
        osInterface (:obj:OSInteract)
        hotspotCommand (str): This command is run through os interface
            to start hotspot service as soon as the config button is pressed.
        timeout (int): Timeout after which the hotspot service gets
            killed. This is just a fail safe if the hotspot service
            takes too long to exit.
        cwd (str): current working directory in which to run the hotspot
            command. Default is set to None in the config interface in
            this case but can be changed later to any other if required.
"""

    def __init__(self, resource, hardwareWatchdog):
        self.resource = resource
        self.osInterface = resource["interface"]["os"]
        self.configInterface = resource["interface"]["config"]
        self.hardwareWatchdog = hardwareWatchdog
        self.hotspotCommand = self.configInterface.get_config(
            "service", "hotspot_config", "start")
        self.timeout = self.configInterface.get_config("service",
                                                       "hotspot_config",
                                                       "timeout")
        self.cwd = self.configInterface.get_config("service", "hotspot_config",
                                                   "cwd")

    def should_hotspot_run(self):
        """
        This checks whether the controller is compatible to run hotspot.
        
        Returns:
            compatibilityCheck (bool): True indicates hotspot service
                should run.
        """
        mode = self.osInterface.get_environment_variable("mode")
        logger.info("HOTSPOT MODE --> {}".format(mode))
        return bool(mode) and (mode != "hotspot")

    def is_hotspot_mode_active(self):
        """
        This tells whether the current mode is hotspot or not. If
        current mode is hotspot, then service manager should not start
        the other irrelevant services unnecessarily.

        Returns:
            hotspotActive (bool)
        """
        mode = self.osInterface.get_environment_variable("mode")
        if not bool(mode):
            return False
        elif mode == "client":
            return False
        elif mode == "hotspot":
            return True

    def start_hotspot(self):
        """
        This starts the hospot by running the subsequent script
        through os interface. The exact script path can be found in the host
        service config file.
        """
        self.osInterface.run_script(self.hotspotCommand, self.timeout,
                                    self.cwd)

    def hotspot_service_run_blocking(self):
        """
        This method is run in a thread since it is a forever
        loop. This loop detects the press of configuration button. If
        pressed it does the following in a sequence:

        - sets hardware watchdog trigger for configReset. This will
            disable the hardware watchdog until the time the controller
            configuration is saved again
        - save mode env variable in the systemd environment
        - starts hotspot service. This will enable and start the
            hotspot services after which the controller gets restarted
            with hotspot network on.
        """

        GPIO_PIN = self.configInterface.get_config("service", "hotspot_config",
                                                   "pin")
        RPi.GPIO.setmode(RPi.GPIO.BCM)
        RPi.GPIO.setup(GPIO_PIN, RPi.GPIO.IN)
        counter = 0
        previousState = 1
        while 1:
            currentState = RPi.GPIO.input(GPIO_PIN)
            if previousState != currentState:
                while 1:
                    time.sleep(1.5)
                    checkingPersistence = RPi.GPIO.input(GPIO_PIN)
                    if currentState == checkingPersistence:
                        counter += 1
                    else:
                        counter = 0
                        break
                    if counter == 3:
                        if self.hardwareWatchdog:
                            self.hardwareWatchdog.change_state("configReset")
                        envFile = self.configInterface.get_service_env_file()
                        self.osInterface.save_systemd_env_variable(
                            envFile, "mode", "hotspot")
                        logger.info("Restarting controller in hotspot mode")
                        self.start_hotspot()
            previousState = currentState
            time.sleep(0.5)

    def start(self):
        """
        This is called by the service manager as it initialises all services.
        Returns:
            startedSuccessfully (bool): True indicates the hotspot
                service has been started successfully.
        
        """
        if self.should_hotspot_run():
            logger.info("Hotspot config service disabled")
            threading.Thread(target=self.hotspot_service_run_blocking).start()
            return True
        else:
            return False

    def stop(self):
        raise NotImplementedError
