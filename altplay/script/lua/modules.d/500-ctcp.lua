local L, msg = require"utils.lambda", require"std.messages"
msg({name = "ctcp", showname = false})

bot.hook("CTCP", function(info)
  if info.message ~= "ACTION" then
    local replies = setmetatable(
      {
        prototype = {
          VERSION     ="gear4's IRC Bot (C) 2017 Aaron Marais",
          FINGER      = ":o MEANIE!",
          TIME        = L[[os.date("%H:%M:%S %a %d %b %Y")]],
          UPTIME      = function()
                          local gett = L[[(_1[_2] ~= 0 or _2 == "sec") and (_1[_2] .. " " .. _2 .. (_1[_2] == 1 and "" or "s") .. (_3 or "")) or ""]]
                          local d = os.date("!*t", os.difftime(os.time(), startTime))
                          return gett(d, "hour", "; ") .. gett(d, "min", "; ") .. gett(d, "sec")
                        end,
          SOURCE      = "https://github.com/",
          SAUCE       = "https://github.com/",
          USERINFO    = "These gosh darn CTCPs",
          CLIENTINFO  = "CLIENTINFO FINGER SOURCE SAUCE VERSION TIME UPTIME USERINFO"
        }
      }, {
        __index = L[[
          local n = rawget(_1, "prototype")[_2]
          return "\001" .. (n ~= nil and (_2.." ") or "ERRMSG ") .. (type(n) == "function" and n() or n or "Invalid CTCP request") .. "\001"
        ]]
      }
    )
    msg.ctcp(replies[info.message:split(" ")[1]]):send("notice", info.nick)
  end
end)