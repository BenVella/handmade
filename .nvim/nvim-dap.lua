local dap = require("dap")

dap.adapters.lldb = {
  type = "executable",
  command = "codelldb",
  name = "lldb"
}

dap.configurations.cpp = {
  {
    name = "Launch handmade with Termux setup",
    type = "lldb",
    request = "launch",
    program = function()
      -- Source your setup script before launching
      vim.fn.system({ "bash", "-c", "source ./scripts/termux11.sh" })
      return "./out/handmade"
    end,
    cwd = "${workspaceFolder}",
    stopOnEntry = false,
    args = {},
    runInTerminal = false,
  },
}
