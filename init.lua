local exports = {}

local function getAddress(aob, errorMsg, modifierFunc)
  local address = core.AOBScan(aob, 0x400000)
  if address == nil then
    log(ERROR, errorMsg)
    error("'winProcHandler' can not be initialized.")
  end
  if modifierFunc == nil then
    return address;
  end
  return modifierFunc(address)
end

exports.enable = function(self, moduleConfig, globalConfig)

  local procAddress = getAddress(
    "83 EC 48 A1 ? ? ? ? 33 C4 89 44 ? ? 8B 44 ? ? 8B 4C ? ? 53 55 56 57",
    "'winProcHandler' was unable to find the main WindowProc address."
  )
  
  local windowCreationProcAddAddr = getAddress(
    "c7 44 24 14 ? ? ? 00 89 7c 24 10",
    "'winProcHandler' was unable to find the point where the main WindowProc is used to create the window.",
    function(foundAddress) return foundAddress + 4 end -- move pointer to actual address
  )

  --[[ load module ]]--
  
  local requireTable = require("winProcHandler.dll") -- loads the dll in memory and runs luaopen_winProcHandler

  -- address of crusaders windowProcCallback needed, fill address of given variable with callback address
  core.writeCode(
    requireTable.address_FillWithWindowProcCallback,
    {procAddress}  -- extreme 1.41.1-E address: 0x004B2C50
  )
  
  -- replaces the function ptr during the window creation with the one from the winProcHandler
  core.writeCode(
    windowCreationProcAddAddr, -- normal 1.41 address: 0x00467aaa
    {requireTable.funcAddress_WindowProc}
  )

end

exports.disable = function(self, moduleConfig, globalConfig) error("not implemented") end

return exports