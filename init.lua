local exports = {}

exports.enable = function(self, moduleConfig, globalConfig)

  local procAddress = core.AOBScan("83 EC 48 A1 ? ? ? ? 33 C4 89 44 ? ? 8B 44 ? ? 8B 4C ? ? 53 55 56 57", 0x400000)
  if procAddress == nil then
    print("'winProcHandler' was unable to find the main WindowProc address.")
    error("'winProcHandler' can not be initialized.")
  end
  
  local windowCreationProcAddAddr = core.AOBScan("c7 44 24 14 ? ? ? 00 89 7c 24 10", 0x400000)
  if windowCreationProcAddAddr == nil then
    print("'winProcHandler' was unable to find the point where the main WindowProc is used to create the window.")
    error("'winProcHandler' can not be initialized.")
  end
  windowCreationProcAddAddr = windowCreationProcAddAddr + 4 -- move pointer to actual address

  --[[ load module ]]--

  local winProcHandler = require("winProcHandler.dll") -- loads the dll in memory and runs luaopen_winProcHandler
  self.winProcHandler = winProcHandler

  -- address of crusaders windowProcCallback needed, fill address of given variable with callback address
  core.writeCode(
    winProcHandler.address_FillWithWindowProcCallback,
    {procAddress}  -- extreme 1.41.1-E address: 0x004B2C50
  )
  
  -- replaces the function ptr during the window creation with the one from the winProcHandler
  core.writeCode(
    windowCreationProcAddAddr, -- normal 1.41 address: 0x00467aaa
    {winProcHandler.funcAddress_WindowProc}
  )

end

exports.disable = function(self, moduleConfig, globalConfig) error("not implemented") end

return exports