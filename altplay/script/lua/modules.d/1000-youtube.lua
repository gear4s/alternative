local L, socket, command, map = require"utils.lambda", require"ssl.https", require"std.commands", require"utils.fp".map, require"json"

local request = function(to, query)
  return
    socket.request(
      ("https://www.googleapis.com/youtube/v3/%s/?key=AIzaSyDSxs701r4sUQKBAaknupg-9WIvSeuJqpI&%s")
      :format(to, table.concat(map.lp(function(k,v) return k.."="..v end, query), "&"))
    )
end

local comma_value = function(amount)
  local formatted = amount
  while true do  
    formatted, k = string.gsub(formatted, "^(-?%d+)(%d%d%d)", '%1,%2')
    if (k==0) then break end
  end
  return formatted
end

bot.hook("PRIVMSG", function(msg)
  if msg.message:match("https?%:%/%/w?w?w?%.?(.+)/watch[&?]v=([^&]*)") then
    local _,_,url,res = msg.message:find("https?%:%/%/w?w?w?%.?(.+)/watch[&?]v=([^&]*)")
    if url == "youtu.be" or url == "youtube.com" then
      local content = request("videos", {id=res, part="snippet,statistics"})
      content = json.decode.decode(content)
      
      if content.pageInfo.totalResults ~= 0 then
        content = content.items[1]
        local str = irc.control.bold .. irc.color.red .. "[ %s ] \003" .. irc.control.bold .. "%s"
        str = str .. " -" .. irc.color.lightgreen .. " %s likes" .. irc.color.reset.. "," .. irc.color.red .. " %s dislikes" .. irc.color.reset
        irc.privmsg(msg.target, str:format(content.snippet.channelTitle, content.snippet.localized.title, comma_value(content.statistics.likeCount), comma_value(content.statistics.dislikeCount)))
      else
        irc.privmsg(msg.target, "Invalid youtube ID")
      end
    end
  end
end)

local cmd = command.add("youtube", function(info) end)

local generateID = function() 
  local id = ""
  local possible = "abcdefghijklmnopqrstuvwxyz0123456789"

  for i = 1, 3 do
    local at = math.floor(math.random() * possible:len())
    id = id .. possible:sub(at,at)
  end

  return id;
end

local sendStr = function(i)
  return  (
            irc.control.bold .. irc.color.red .. "[ %s ] \003" .. irc.control.bold .. "%s" .. irc.color.reset ..
            " - https://youtube.com/watch?v=%s"
          ):format(
            i.snippet.channelTitle, i.snippet.title, i.id.videoId
          )
end

cmd.addsubcmd("search", function(info)
  local msg = info.msg.message:split(" ")
  table.remove(msg, 1)
  table.remove(msg, 1)
  msg = table.concat(msg, " ")
  local content = request("search", {q=msg, part="id,snippet", type="video"})
  
  for k,v in ipairs(json.decode.decode(content).items) do
    irc.privmsg(info.msg.target, sendStr(v))
  end
end)

cmd.addsubcmd("random", function(info)
  local content = request("search", {q=generateID(), part="id,snippet", type="video", maxResults=50})
  content = json.decode.decode(content).items
  irc.privmsg(info.msg.target, sendStr(content[math.floor(math.random() * #content)]))
end)

command.alias("yt", "youtube")