import inspect

def test_app(app):
    print("\n")
    print("App path: ",app.app_path)
    print("Binary path", app.binary_path)
    print("Get Binary path", app._get_binary_path())
    
def test_dut(dut):
    print(inspect.signature(dut.expect))