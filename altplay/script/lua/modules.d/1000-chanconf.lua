--[[
  
  Client-side interface for per-channel configuration
  
]]

local commands, chanconfig = require"std.commands", require"std.chanconfig"
chanconfig.init()

local cmd = commands.add("set", function() end)

cmd.addsubcmd("prefix", function(info)
              print(info.args)
  local lcmd = info.args:lower():match("^([%l%d]*)")
  chanconfig.set(info, "prefix", lcmd[1])
end)

local cmd = commands.add("channel", function() end)
cmd.addsubcmd("add", function(info)
  
end)
cmd.addsubcmd("set", function(info)
end)

print("Loaded channel config successfully")