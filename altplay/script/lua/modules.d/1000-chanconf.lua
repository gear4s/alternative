--[[
  
  Client-side interface for per-channel configuration
  
]]

local commands, chanconfig = require"std.commands", require"std.chanconfig"
chanconfig.init()

local cmd = commands.add("set", function() end)

cmd.addsubcmd("prefix", function(info)
  local lcmd = info.args:lower():match("^([%l%d]*)")
  chanconfig.set(info, "prefix", lcmd)
end)

local cmd = commands.add("channel", function() end)
cmd.addsubcmd("add", function(info)
  chanconfig.new(info)
end)
cmd.addsubcmd("set", function(info)
  info.args = info.args:split(" ")
  chanconfig.set(info, info.args[2], info.args[3])
end)

print("Loaded channel config successfully")