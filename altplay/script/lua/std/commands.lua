--[[

  Parse user commands.

]]--

local module = {}
local cmdhooks = setmetatable({c={},a={}}, {
  __index = function(t,k)
    return rawget(t, "c")[k] or rawget(t, "a")[k]
  end
})

local fp,msg = require"utils.fp",require"std.messages"
msg({name = "command", color = "${darkblue}", keepers = {prefix = " ${blue}[ ", suffix = " ${blue}] ${reset}"}})

local map, I = fp.map, fp.I

local function checkstring(info, prefix)
  local full, lcmd = info.message:lower():match("^( *" .. prefix .. "([%l%_%d]+) *)")
  if not full or lcmd == "" then return end
  if not cmdhooks[lcmd] then return msg.command("${red}Unrecognized command: ${cmdname}"):format({cmdname = lcmd}):send("notice", info.nick) end
  if cmdhooks[lcmd].a then
    lcmd = cmdhooks[lcmd].cb
  end
  local args = info.message:sub(#full + 1):split(" ")
  local cb, extra
  if args[1] then
    cb = cmdhooks[lcmd]
    if cb.scmd and cb.scmd[args[1]] then
      cb = cb.scmd[args[1]].cb
      extra = args[1]
      table.remove(args, 1)
    else
      cb = cb.cb
    end
  end
  (cb or function() msg.command("${red}Unrecognized command: ${cmdname}${extra}"):format({cmdname = lcmd, extra = extra or ""}):send("notice", info.nick) end)({ msg = info, command = lcmd, args = args })
end
bot.hook("PRIVMSG", function(info) checkstring(info, "%#") end)

local usageFormatter = function(usage)
  local ustr = ""
  if usage then
    for k,v in pairs(usage:split(";")) do
      local t = v:split(":")
      if t[1] == "r" then
        for k,v in pairs(t[2]:split(",")) do
          ustr = ustr .. "<"..v.."> "
        end
      elseif t[1] == "o" then
        for k,v in pairs(t[2]:split(",")) do
          ustr = ustr .. "["..v.."] "
        end
      end
    end
  end
  return ustr
end
module.add = function(cmdname, cmdcallback, help, usage)
  cmdhooks.c[cmdname] = {cb=cmdcallback, h=help, u=usage, scmd={}}
  cmdhooks.c.help.scmd[cmdname] = {scmd = {}, cb=function(info, pre)
    irc.notice(info.msg.nick, (pre or "") .. (cmdhooks[cmdname].h or "No help for this command."))
    irc.notice(info.msg.nick, "Usage: #" .. cmdname .. " " .. usageFormatter(usage))
    if next(cmdhooks.c[cmdname].scmd, nil) and cmdname ~= "help" then
      local subcmds = {}
      for k,v in pairs(cmdhooks.c[cmdname].scmd) do
        subcmds[#subcmds+1] = k
      end
      msg.command("Sub-commands for ${darkgray}${cmdname}${reset}: ${cmdsub}"):format({cmdname = cmdname, cmdsub = table.concat(subcmds, " ")}):send("notice", info.msg.nick)
    end
  end}
  return {
    addsubcmd = function(scmdname, scmdcallback, shelp, usage)
      cmdhooks.c[cmdname].scmd[scmdname] = {cb=scmdcallback}
      cmdhooks.c.help.scmd[cmdname].scmd[scmdname] = {
        cb = function(info, pre)
          irc.notice(info.msg.nick, (pre or "") .. (shelp or "No help for this command."))
          irc.notice(info.msg.nick, "Usage: #" .. cmdname .. " " .. scmdname .. " " .. usageFormatter(usage))
        end
      }
    end
  }
end

module.alias = function(cmdname, cmdcallback)
  cmdhooks.a[cmdname] = {a=true,cb=cmdcallback}
  cmdhooks.c.help.scmd[cmdname] = {cb=function(info) cmdhooks.c.help.scmd[cmdcallback].cb(info, "Alias to " .. cmdcallback .. ": ") end}
end

module.add("help", function(info)
  if info.args ~= "" then return irc.notice(info.msg.nick, "Unrecognized command") end
  local lcmd = info.args:lower():match("^([%l%d]*)")
  msg.command("Commands: ${darkgray}${commands}"):format({commands = table.concat(table.sort(map.lp(I, cmdhooks.c)), ", ")}):send("notice", info.msg.nick)
  local cmds = table.concat(table.sort(map.lp(I, cmdhooks.a)), ", ")
  if cmds ~= "" then
    msg.command("Command Aliases: ${darkgray}${cmds}"):format({cmds = cmds}):send("notice", info.msg.nick)
  end
end, "Returns information on commands", "o:command")

module.alias("h", "help")

return module
