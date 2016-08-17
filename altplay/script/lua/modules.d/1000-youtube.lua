local L, socket, command, map = require"utils.lambda", require"ssl.https", require"std.commands", require"utils.fp".map, require"json"

local request = function(to, query)
  return
    socket.request(
      ("https://www.googleapis.com/youtube/v3/%s/?key=AIzaSyDSxs701r4sUQKBAaknupg-9WIvSeuJqpI&%s")
      :format(to, table.concat(map.lp(function(k,v) return k.."="..v end, query), "&"))
    )
end

bot.hook("PRIVMSG", function(msg)
  if msg.message:match("https?%:%/%/w?w?w?%.?(.+)/watch[&?]v=([^&]*)") then
    local _,_,url,res = msg.message:find("https?%:%/%/w?w?w?%.?(.+)/watch[&?]v=([^&]*)")
    if url == "youtu.be" or url == "youtube.com" then
      local content = request("videos", {id=res, part="snippet,statistics"})
      content = json.decode.decode(content).items[1]
      
      local str = irc.control.bold .. irc.color.red .. "[ %s ] \003" .. irc.control.bold .. "%s"
      str = str .. " -" .. irc.color.lightgreen .. " %i likes" .. irc.color.reset.. "," .. irc.color.red .. " %i dislikes" .. irc.color.reset
      irc.msg("altplay.dev", str:format(content.snippet.channelTitle, content.snippet.localized.title, content.statistics.likeCount, content.statistics.dislikeCount))
    end
  end
end)

local cmd = command.add("youtube", function(info)
  irc.privmsg(info.msg.target, "got youtube")
end)

cmd.addsubcmd("search", function(info)
  irc.privmsg(info.msg.target, "got search")
end)

cmd.addsubcmd("random", function(info)
  
end)

command.alias("yt", "youtube")