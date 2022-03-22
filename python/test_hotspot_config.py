import pytest
from unittest.mock import Mock, MagicMock, patch, mock_open
from .test_helper import *

rpi = MagicMock()
rpi_gpio = MagicMock()
modules = {"RPi": rpi, "RPi.GPIO": rpi_gpio}
modulePatcher = patch.dict("sys.modules", modules)
modulePatcher.start()
from ..hotspot_config import HotspotConfig


@pytest.fixture()
def hotspot_config():
    resource = generate_mock_resource()
    hardwareWatchdog = Mock()
    hotspotConfig = HotspotConfig(resource, hardwareWatchdog)

    return hotspotConfig


def test_shouldHotspotRun_modeDoesntExist_shouldNotRun(hotspot_config):
    hotspot_config.osInterface.get_environment_variable.return_value = None

    shouldRun = hotspot_config.should_hotspot_run()

    assert shouldRun is False


def test_shouldHotspotRun_modeIsHotspot_shouldNotRun(hotspot_config):
    hotspot_config.osInterface.get_environment_variable.return_value = "hotspot"

    shouldRun = hotspot_config.should_hotspot_run()

    assert shouldRun is False


def test_shouldHotspotRun_modeIsClient_shouldRun(hotspot_config):
    hotspot_config.osInterface.get_environment_variable.return_value = "client"

    shouldRun = hotspot_config.should_hotspot_run()

    assert shouldRun is True


@patch("hostServices.services.hotspot_config.time")
@patch("hostServices.services.hotspot_config.RPi.GPIO")
def test_hotspotServiceRunning_buttonPressed(mockGPIO, mockTime,
                                             hotspot_config):
    mockTime.sleep.side_effect = [1, 1, 1, 1, 1, 1, 1, Exception]
    mockGPIO.input.side_effect = [1, 1, 1, 0, 0, 0, 0]
    hotspot_config.configInterface.get_service_env_file = Mock(
        return_value="temp")

    with pytest.raises(Exception):
        hotspot_config.hotspot_service_run_blocking()

    hotspot_config.osInterface.run_script.assert_called_once_with(
        hotspot_config.hotspotCommand, hotspot_config.timeout,
        hotspot_config.cwd)


@patch("hostServices.services.hotspot_config.time")
@patch("hostServices.services.hotspot_config.RPi.GPIO")
def test_hotspotServiceRunning_hardwareWatchdogCompatible_changeState(
        mockGPIO, mockTime, hotspot_config):
    mockTime.sleep.side_effect = [1, 1, 1, 1, 1, 1, 1, Exception]
    mockGPIO.input.side_effect = [1, 1, 1, 0, 0, 0, 0]
    hotspot_config.configInterface.get_service_env_file = Mock(
        return_value="temp")
    hotspot_config.hardwareWatchdog = Mock()

    with pytest.raises(Exception):
        hotspot_config.hotspot_service_run_blocking()

    hotspot_config.hardwareWatchdog.change_state.assert_called_once_with(
        "configReset")


@patch("hostServices.services.hotspot_config.time")
@patch("hostServices.services.hotspot_config.RPi.GPIO")
def test_hotspotServiceRunning_hardwareWatchdogCompatible_saveModeEnvVariable(
        mockGPIO, mockTime, hotspot_config):
    mockTime.sleep.side_effect = [1, 1, 1, 1, 1, 1, 1, Exception]
    mockGPIO.input.side_effect = [1, 1, 1, 0, 0, 0, 0]
    hotspot_config.configInterface.get_service_env_file = Mock(
        return_value="temp")
    hotspot_config.hardwareWatchdog = Mock()

    with pytest.raises(Exception):
        hotspot_config.hotspot_service_run_blocking()

    hotspot_config.osInterface.save_systemd_env_variable.assert_called_once_with(
        "temp", "mode", "hotspot")


@patch("hostServices.services.hotspot_config.threading")
def test_start_yesHotspotShouldRun_startHotspotThread(mockThread,
                                                      hotspot_config):
    hotspot_config.hotspot_service_run_blocking = Mock()
    hotspot_config.should_hotspot_run = Mock(return_value=True)

    hotspot_config.start()

    mockThread.Thread().start.assert_called_once()


@patch("hostServices.services.hotspot_config.threading")
def test_start_yesHotspotShouldRun_returnTrue(mockThread, hotspot_config):
    hotspot_config.hotspot_service_run_blocking = Mock()
    hotspot_config.should_hotspot_run = Mock(return_value=True)

    startSuccess = hotspot_config.start()

    assert startSuccess is True


@patch("hostServices.services.hotspot_config.threading")
def test_start_yesHotspotShouldRun_returnTrue(mockThread, hotspot_config):
    hotspot_config.hotspot_service_run_blocking = Mock()
    hotspot_config.should_hotspot_run = Mock(return_value=False)

    startSuccess = hotspot_config.start()

    assert startSuccess is False


@pytest.mark.parametrize("mode", [(None), ("")])
def test_isHotspotModeActive_controllerNotHotspotCompatible_returnFalse(
        mode, hotspot_config):
    hotspot_config.osInterface.get_environment_variable.return_value = mode

    hotspotActive = hotspot_config.is_hotspot_mode_active()

    assert hotspotActive is False


@pytest.mark.parametrize("mode", [("client")])
def test_isHotspotModeActive_clientMode_returnFalse(mode, hotspot_config):
    hotspot_config.osInterface.get_environment_variable.return_value = mode

    hotspotActive = hotspot_config.is_hotspot_mode_active()

    assert hotspotActive is False


@pytest.mark.parametrize("mode", [("hotspot")])
def test_isHotspotModeActive_hotspotMode_returnFalse(mode, hotspot_config):
    hotspot_config.osInterface.get_environment_variable.return_value = mode

    hotspotActive = hotspot_config.is_hotspot_mode_active()

    assert hotspotActive is True
