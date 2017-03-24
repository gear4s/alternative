if os.getenv"CHILDPROC" then return end

local L, command, msg = require"utils.lambda", require"std.commands", require"std.messages"
local lfs = require"lfs"

msg({name = "child", color = "${darkred}", keepers = {prefix = "${red}>>>", suffix = ""}, showname = false})

local baseCmd = command.add("child")
baseCmd.addsubcmd("add", function(info)
  if not lfs.isDir(name) and os.rename(name,name) then return end
  
  local args = info.args:split(" ")
  if #args ~= 2 then return end
  
end, "Adds a child process to the list", "Usage: #child add <name> <channel>")

baseCmd.addsubcmd("list", function(info)
  
end, "Lists all processes that have been started, and which channel they reside in", "Usage: #child list")