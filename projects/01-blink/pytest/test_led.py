import pytest
from led_model import LED_PIN

class BlinkLed:
    def __init__(self):
        self.led_state = None
        
    def on(self):
        self.led_state = 1

    def off(self):
            self.led_state = 0

@pytest.fixture() # (scope="module")it helps control instances in the case of expensive resources invocation, its best to create one instance.
def led():
    print("\nCreating LED")
    yield BlinkLed()
    print("\nCleaning up LED")


def test_led_on(led):
    led.on()
    assert led.led_state == 1 
    
def test_led_off(led):
    led.off()
    method = getattr(led,"off")
    print("method: ",method)
    assert led.led_state == 0
    
@pytest.mark.parametrize(
    "operation, expected",
    [
        ("off", 0),
        ("on", 1),
        ("on", 1)
    ]
)
def test_many_leds(led,operation,expected):
    getattr(led,operation)() # invokes the method in BlinkLed class => 'off' = led.off()
    assert led.led_state == expected