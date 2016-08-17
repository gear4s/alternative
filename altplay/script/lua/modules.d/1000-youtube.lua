local L, socket = require"utils.lambda", require"ssl.https", require"json"

local baseUrl = "https://www.googleapis.com/youtube/v3"

bot.hook("PRIVMSG", function(msg)
  if msg.message:match("https?%:%/%/w?w?w?%.?(.+)/watch[&?]v=([^&]*)") then
    local _,_,url,res = msg.message:find("https?%:%/%/w?w?w?%.?(.+)/watch[&?]v=([^&]*)")
    if url == "youtu.be" or url == "youtube.com" then
      local content = socket.request(baseUrl.."/videos/?key=AIzaSyDSxs701r4sUQKBAaknupg-9WIvSeuJqpI&id="..res.."&part=snippet,statistics")
      content = json.decode.decode(content)
      
      local str = irc.control.bold .. irc.color.red .. "[ " .. content.items[1].snippet.channelTitle .. " ] \003" .. irc.control.bold .. content.items[1].snippet.localized.title
      str = str .. " - " .. content.items[1].statistics.likeCount .. " likes, " .. content.items[1].statistics.dislikeCount .. " dislikes"
      irc.msg("altplay.dev", str)
    end
  end
end)