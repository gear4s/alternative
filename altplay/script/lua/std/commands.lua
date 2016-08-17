--[[

  Parse user commands.

]]--

local module = {}
local cmdhooks = setmetatable({c={},a={}}, {
  __index = function(t,k)
    return rawget(t, "c")[k] or rawget(t, "a")[k]
  end
})

local fp = require"utils.fp"
local map, I = fp.map, fp.I

local function checkstring(info, prefix)
  local full, lcmd = info.message:lower():match("^( *" .. prefix .. "([%l%_%d]+) *)")
  if not full or lcmd == "" then return end
  if not cmdhooks[lcmd] then return irc.notice(info.nick, "Unrecognized command " .. lcmd) end
  if cmdhooks[lcmd].a then
    lcmd = cmdhooks[lcmd].cb
  end
  local args = info.message:sub(#full + 1)
  local subcmd = args:split(" ")
  local cb = subcmd[1] and cmdhooks[lcmd].scmd[subcmd[1]] and cmdhooks[lcmd].scmd[subcmd[1]].cb or cmdhooks[lcmd].cb
  cb({ msg = info, command = lcmd, args = args })
end

bot.hook("PRIVMSG", function(info) checkstring(info, "%./") end)

module.add = function(cmdname, cmdcallback, help)
  cmdhooks.c[cmdname] = {cb=cmdcallback, h=help, scmd={}}
  return {
    addsubcmd = function(scmdname, scmdcallback, shelp)
      cmdhooks.c[cmdname].scmd[scmdname] = {cb=scmdcallback, h=shelp}
    end
  }
end

module.alias = function(cmdname, cmdcallback, help)
  cmdhooks.a[cmdname] = {a=true,cb=cmdcallback, h=help}
end

module.add("help", function(info)
  local lcmd = info.args:lower():match("^([%l%d]*)")
  if lcmd == "" then
    local cmds = table.concat(table.sort(map.lp(I, cmdhooks.c)), ", ")
    irc.notice(info.msg.nick, cmds ~= "" and ("Commands: " .. cmds) or "Unrecognized command")
    cmds = table.concat(table.sort(map.lp(I, cmdhooks.a)), ", ")
    if cmds ~= "" then
      irc.notice(info.msg.nick, "Command Aliases: " .. cmds)
    end
    return
  end
  if not cmdhooks[lcmd] then return irc.notice(info.msg.nick, "Unrecognized command") end
  irc.notice(info.msg.nick, cmdhooks[lcmd].h or "No help for this command.")
end, "Usage: #help [command]")

module.alias("h", "help")

return module
