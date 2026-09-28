import inspect

def test_app(app):
    print("\n")
    print("App path: ",app.app_path)
    print("Binary path", app.binary_path)
    print("Get Binary path", app._get_binary_path())
    
def test_dut(dut):
    text = "LED OFF"
    match = dut.expect(text, timeout=5) # the timeout allows for a break in loop after n seconds
    assert match.group().decode() == text

def test_blink_sequence(dut): # test the actual sequence in serial buffer
    dut.expect("Initialized", timeout=5)
    dut.expect("LED ON", timeout=5)
    

    dut.expect("LED OFF", timeout=5)
    
    
def test_blink_off_sequence(dut): # test the actual sequence in serial buffer
    dut.expect("LED ON", timeout=5)
    print("FOUND ON")
    dut.expect("Initialized", timeout=5)
    print("FOUND INIT")
    dut.expect("LED OFF", timeout=5)
    print("FOUND OFF")