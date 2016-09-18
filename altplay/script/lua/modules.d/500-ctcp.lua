local L, msg = require"utils.lambda", require"std.messages"
msg({name = "ctcp", showname = false})

bot.hook("CTCP", function(info)
  if info.message ~= "ACTION" then
    local replies = setmetatable(
      {
        prototype = {
          VERSION     =" AltPlay IRC Bot (C) 2016 AltPlay Community Developers",
          FINGER      = ":o MEANIE!",
          TIME        = L([[os.date("%H:%M:%S %a %d %b %Y")]]),
          UPTIME      = function()
                          local d = os.date("!*t", os.difftime(os.time(), startTime))
                          return d.hour .. " hours; " .. d.min .. " minutes; " .. d.sec .. " seconds"
                        end,
          SOURCE      = "http://altplay.net/",
          SAUCE       = "http://altplay.net/",
          USERINFO    = "These gosh darn CTCPs",
          CLIENTINFO  = "CLIENTINFO FINGER SOURCE SAUCE VERSION TIME UPTIME USERINFO"
        }
      }, {
        __index = L([[
          local n = rawget(_1, "prototype")[_2]
          return "\001" .. (n ~= nil and (_2.." ") or "ERRMSG ") .. (type(n) == "function" and n() or n or "Invalid CTCP request") .. "\001"
        ]])
      }
    )
    msg.ctcp(replies[info.message:split(" ")[1]]):send("notice", info.nick)
  end
end)