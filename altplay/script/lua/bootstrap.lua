print(irc.error.NOSUCHNICK)

bot.hook(396, function(msg)
  if msg.is_server_message then
    bot.later(5000, function()
      print("later server message\n\n")
    end()
    bot.send("NICK lua_tester")
  end
end)

bot.hook("NOTICE", function(msg)
  print(msg.nick .. " at " .. msg.hostmask .. " says " .. msg.message)
  
  print((bot.quit and "t" or "f").."\n\n")
  bot.quit = true
  print((bot.quit and "t" or "f").."\n\n")
  bot.quit = false
end)