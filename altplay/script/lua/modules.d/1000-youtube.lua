local L, socket, command, map, msg = require"utils.lambda", require"ssl.https", require"std.commands", require"utils.fp".map, require"std.messages", require"json"
msg({name = "youtube", color = "${red}", keepers = {prefix = " ${blue}[ ", suffix = " ${blue}] ${reset}"}})

local request = function(to, query)
    return socket.request(
      ("https://www.googleapis.com/youtube/v3/%s/?key=AIzaSyDSxs701r4sUQKBAaknupg-9WIvSeuJqpI&%s")
      :format(to, table.concat(map.lp(function(k,v) return k.."="..v end, query), "&"))
    )
end

local comma_value = function(amount)
  local formatted, k = amount
  while true do  
    formatted, k = string.gsub(formatted, "^(-?%d+)(%d%d%d)", '%1,%2')
    if k == 0 then break end
  end
  return formatted
end

bot.hook("PRIVMSG", function(info)
  if info.message:match("https?%:%/%/w?w?w?%.?(.+)/watch[&?]v=([^&]*)") then
    local _,_,url,res = info.message:find("https?%:%/%/w?w?w?%.?(.+)/watch[&?]v=([^&]*)")
    if url == "youtu.be" or url == "youtube.com" then
      local content = request("videos", {id=res, part="snippet,statistics"})
      content = json.decode.decode(content)
      
      if content.pageInfo.totalResults ~= 0 then
        content = content.items[1]
        msg.youtube("${bold}${red}[ ${ctitle} ] ${reset}${bold}${vtitle} - ${lightgreen} ${lcount} likes${reset},${red} ${dcount} dislikes")
          :format({ctitle = content.snippet.channelTitle, vtitle = content.snippet.localized.title, lcount = comma_value(content.statistics.likeCount), dcount = comma_value(content.statistics.dislikeCount)})
          :toggle("showname")
          :send("privmsg", info.target)
      end
    end
  end
end)

local cmd = command.add("youtube")

local generateID = function() 
  local id = ""
  local possible = "abcdefghijklmnopqrstuvwxyz0123456789"

  for i = 1, 3 do
    local at = math.floor(math.random() * possible:len())
    id = id .. possible:sub(at,at)
  end

  return id;
end

local sendStr = function(i, target)
  msg.youtube("${bold}${red}[ ${ctitle} ] ${reset}${bold}${vtitle}${reset} - https://youtube.com/watch?v=${vid}")
    :format({ctitle = i.snippet.channelTitle, vtitle = i.snippet.title, vid = i.id.videoId})
    :send("privmsg", target)
end

local urlencode = function(str)
  if (str) then
    str = string.gsub(str, "\n", "\r\n")
    str = string.gsub(str, "([^%w ])", L'string.format("%%%02X", string.byte(_1)) end')
    str = string.gsub(str, " ", "+")
  end
  return str    
end
cmd.addsubcmd("search", function(info)
  local msg = info.msg.message:split(" ")
  table.remove(msg, 1)
  table.remove(msg, 1)
  msg = table.concat(msg, " ")
  local content = request("search", {q=urlencode(msg), part="id,snippet", type="video", maxResults=3})
  
  for k,v in ipairs(json.decode.decode(content).items) do
    sendStr(v, info.msg.target)
  end
end, "Searches YouTube, requests the ID and a snippet. 3 results maximum", "r:string...")

cmd.addsubcmd("random", function(info)
  local content = request("search", {q=generateID(), part="id,snippet", type="video", maxResults=50})
  content = json.decode.decode(content).items
  sendStr(content[math.floor(math.random() * #content)], info.msg.target)
end, "Requests a random YouTube video using random possible video titles")

command.alias("yt", "youtube")