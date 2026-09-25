# inval = 4
# check = 5

# def inc(x):
#     return x + 1

# def test_answer():
#     assert inc(inval) == check
    

class BlinkLed:
    def __init__(self):
        self.Led_state = False
        
    def led_on(self,pin):
        self.Led_state = 1
        print(f"making pin {pin} == {self.Led_state}")
        return self.Led_state

    def led_off(self, pin):
            self.Led_state = 0
            print(f"making pin {pin} == {self.Led_state}")
            return self.Led_state

val = BlinkLed()

def test_answer_1():
    assert val.led_on(2) == 1
    
def test_answer_2():
    assert val.led_off(2) == 0

