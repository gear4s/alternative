--ease require"my.module"
package.path = "./script/?.lua;" .. package.path

local startTime = os.time()

print(("t f"):split(" ")[1])

print(irc.error.NOSUCHNICK)

bot.hook(396, function(msg)
  if msg.is_server_message then
    irc.join("#altplay.dev", "")
    
    bot.later(5000, function()
      irc.nick("luaBotTester")
      irc.msg("altplay.dev", "I am lua_tester :) thanks! im idle now")
    end)
  end
end)

bot.hook("PRIVMSG", function(msg)
  irc.msg("altplay.dev", msg.nick .. " at " .. msg.hostmask .. " on " .. msg.target .. " says " .. msg.message)
end)

bot.hook("CTCP", function(msg)
  if msg.message ~= "ACTION" then
    irc.notice(msg.nick, (setmetatable({
      prototype={
        VERSION="AltPlay IRC Bot (C) 2016 AltPlay Community Developers",
        FINGER=":o MEANIE!",
        TIME=function() return os.date("%H:%M:%S %a %d %b %Y") end,
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
