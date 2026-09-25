import pytest

class BlinkLed:
    def __init__(self):
        self.led_state = None
        
    def on(self):
        self.led_state = 1

    def off(self):
            self.led_state = 0

@pytest.fixture() # it helps control instances in the case of expensive resources invocation, its best to create one instance.
def led():
    print("\nCreating LED")
    yield BlinkLed()
    print("\nCleaning up LED")


def test_led_on(led):
    led.on()
    assert led.led_state == 1 
    
def test_led_off(led):
    led.off()
    assert led.led_state == 0
    