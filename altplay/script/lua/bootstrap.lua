--ease require"my.module"
package.path = "./script/?.lua;" .. package.path

print(("t/f"):split("/")[1])

print(irc.error.NOSUCHNICK)

bot.hook(396, function(msg)
  if msg.is_server_message then
    bot.send("NICK luabot1")
    bot.send("JOIN #altplay.dev")
    bot.send("PRIVMSG #altplay :I am lua_tester :) thanks! im idle now")
  end
end)

bot.hook("NOTICE", function(msg)
  print(msg.nick .. " at " .. msg.hostmask .. " says " .. msg.message)
  
  print((bot.quit and "t" or "f").."\n\n")
  bot.quit = true
  print((bot.quit and "t" or "f").."\n\n")
  bot.quit = false
end)

bot.hook("PRIVMSG", function(msg)
  print(msg.nick .. " at " .. msg.hostmask .. " says " .. msg.message)
end)