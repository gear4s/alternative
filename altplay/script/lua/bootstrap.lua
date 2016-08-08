--ease require"my.module"
package.path = "./script/?.lua;" .. package.path

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
  if msg.message:find("\001") == 1 then -- CTCP
    irc.msg("altplay.dev", "CTCP request from " .. msg.nick .. ": " .. table.concat(msg.message:gsub("\001", ""):split(" "), " ; "))
  else
    irc.msg("altplay.dev", msg.nick .. " at " .. msg.hostmask .. " on " .. msg.target .. " says " .. msg.message)
  end
end)