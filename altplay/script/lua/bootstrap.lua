--ease require"my.module"
package.path = "./script/lua/?.lua;" .. package.path

local startTime = os.time()
local L = require"utils.lambda"

print(("t f"):split(" ")[1])

print(irc.error.NOSUCHNICK)

bot.hook(396, function(msg)
  if msg.is_server_message then
    irc.join("#altplay.dev", "")
    
    bot.later(5000, L("irc.nick('luaBotTester');irc.msg('altplay.dev', 'I am lua_tester :) thanks! im idle now')"))
  end
end)

bot.hook("PRIVMSG", L([[
  irc.msg("altplay.dev", _1.nick .. " at " .. _1.hostmask .. " on " .. _1.target .. " says " .. _1.message)
]]))

bot.hook("CTCP", function(msg)
  if msg.message ~= "ACTION" then
    irc.notice(msg.nick, (setmetatable({
      prototype={
        VERSION="AltPlay IRC Bot (C) 2016 AltPlay Community Developers",
        FINGER=":o MEANIE!",
        TIME=L([[os.date("%H:%M:%S %a %d %b %Y")]]),
        UPTIME=function()
          local d = os.date("!*t", os.difftime(os.time(), startTime))
          return d.hour .. " hours; " .. d.min .. " minutes; " .. d.sec .. " seconds"
        end,
        SOURCE="http://altplay.net/",
        SAUCE="http://altplay.net/",
        USERINFO="These gosh darn CTCPs",
        CLIENTINFO="CLIENTINFO FINGER SOURCE SAUCE VERSION TIME UPTIME USERINFO"
      }
    }, {
      __index = function(t,k)
        local n = rawget(t, "prototype")[k]
        return "\001" .. (n ~= nil and (k.." ") or "ERRMSG ") .. (type(n) == "function" and n() or n or "Invalid CTCP request") .. "\001"
      end
    }))[msg.message:split(" ")[1]])
  end
end)
