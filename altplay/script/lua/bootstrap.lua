--ease require"my.module"
package.path = "./script/?.lua;" .. package.path

print(("t/f"):split("/")[1])

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