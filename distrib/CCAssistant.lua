local guid = "61062d79-F1EB-4AFC-A3C3-AA2C0CD6375B"

local function CCAssistantExist()
  return Plugin.Exist(guid)
end

local function CCAssistantCall(macro_command_name)
  return Plugin.Call(guid, macro_command_name)
end

--[[
Macro {
  area = "Editor";
  key = "?";
  description = "Configure the plugin";
  condition = CCAssistantExist;
  action = function() CCAssistantCall("Configure") end;
}
]]--
