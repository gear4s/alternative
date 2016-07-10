bot.hook("396", function(msg)
  if msg.servmsg then
    print("server message\n\n")
  else
    print(msg.nick)
  end
  
  print((bot.quit and "t" or "f").."\n\n")
  bot.quit = true
  print((bot.quit and "t" or "f").."\n\n")
  bot.quit = false
end)