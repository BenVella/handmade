local dap = require("dap")

-- codelldb must be installed directly in termux
-- run `pkg install x11-repo && pkg isntall codelldb`
dap.adapters.lldb = {
  type = "executable",
  command = "codelldb",
  name = "lldb"
}

dap.configurations.cpp = {
  {
    name = "Launch via shell script",
    type = "lldb",
    request = "launch",
    program = "${workspaceFolder}/out/handmade",
    cwd = "${workspaceFolder}",
    runInTerminal = true,
    env = {
      DISPLAY = ":1",
    },
    stopOnEntry = false,
  },
}

