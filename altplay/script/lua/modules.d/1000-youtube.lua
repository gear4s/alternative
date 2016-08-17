local L, socket = require"utils.lambda", require"ssl.https", require"json"

local baseUrl = "https://www.googleapis.com/youtube/v3"

bot.hook("PRIVMSG", function(msg)
  if msg.message:match("https?%:%/%/w?w?w?%.?(.+)/watch[&?]v=([^&]*)") then
    local _,_,url,res = msg.message:find("https?%:%/%/w?w?w?%.?(.+)/watch[&?]v=([^&]*)")
    if url == "youtu.be" or url == "youtube.com" then
      local content = socket.request(baseUrl.."/videos/?key=AIzaSyDSxs701r4sUQKBAaknupg-9WIvSeuJqpI&id="..res.."&part=snippet,statistics")
      content = json.decode.decode(content).items[1]
      
      local str = irc.control.bold .. irc.color.red .. "[ %s ] \003" .. irc.control.bold .. "%s"
      str = str .. " -" .. irc.color.lightgreen .. " %i likes" .. irc.color.reset.. "," .. irc.color.red .. " %i dislikes" .. irc.color.reset
      irc.msg("altplay.dev", str:format(content.snippet.channelTitle, content.snippet.localized.title, content.statistics.likeCount, content.statistics.dislikeCount))
    end
  end
end)