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
        VERSION="VERSION AltPlay IRC Bot (C) 2016 AltPlay Community Developers",
        FINGER="FINGER :o MEANIE!",
        TIME=function() return os.date("%H:%M:%S %a %d %b %Y") end,
        UPTIME=function()
          local d = os.date("!*t", os.difftime(os.time(), startTime))
          return "UPTIME " .. d.hour .. " hours; " .. d.min .. " minutes; " .. d.sec .. " seconds"
        end,
        SOURCE="SOURCE http://altplay.net/",
        SAUCE="SAUCE http://altplay.net/",
        USERINFO="USERINFO These gosh darn CTCPs",
        CLIENTINFO="CLIENTINFO CLIENTINFO FINGER SOURCE SAUCE VERSION TIME UPTIME USERINFO"
      }
    }, {
      __index = function(t,k)
        return "\001" .. (type(rawget(t,"prototype")[k]) == "function" and rawget(t,"prototype")[k]() or rawget(t,"prototype")[k] or "ERRMSG Invalid CTCP request") .. "\001"
      end
    }))[msg.message:split(" ")[1]])
  end
end)