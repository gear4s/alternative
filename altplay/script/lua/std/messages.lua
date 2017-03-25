--[[
  Messages module
  from gear4
]]

local L = require"utils.lambda"
local msgd = {
  msg = "",
  name = "",
  color = "\0032",
  showname = true,
  shownamecolor = true,
  fmt = {},
  keepers = {
    prefix = "",
    suffix = " "
  },
  colors = irc.color,
  control = irc.control,
  
  __call = function(self, ...)
    local tbl = setmetatable({}, {__index = self})
    tbl.msg = tbl.msgcb and tbl:msgcb(...) or select(1, ...)
    return tbl
  end,
  
  format = function(self, args)
    for k,v in pairs(args) do
      if type(args[k]) == "table" then
        return {
          send = function()
            error("index " .. k .. " (msg:format) is table; please contact developer.")
          end
        }
      end
    end
    self.fmt = args
    return self
  end,
  
  toggle = function(self, arg)
    self[arg] = not self[arg]
    return self
  end,
  
  preprocess = function(self)
    local proc = function(s)
      if s == nil then return "" end
      
      return s:gsub('($%b{})', function(w)
        w = w:sub(3, -2)
        return self.colors[w] or self.control[w] or self.fmt[w] or w
      end)
    end
    
    local msg =  (self.showname and (proc(self.keepers.prefix .. "${reset}"))                                            or "")
    msg = msg .. (self.showname and (proc(self.shownamecolor and self.color or "") .. (self.name):upper()) or "")
    msg = msg .. (self.showname and (proc("${reset}" .. self.keepers.suffix .. "${reset}"))                                            or "")
    msg = msg ..  proc(self.msg)
    
    return msg
  end,
  
  send = function(self, t, n)
    irc[t](n, self:preprocess())
  end
}
msgd.__index = msgd

local msgs = setmetatable({}, {
  __call = function(self, name)
    local msgc = setmetatable({}, msgd)
    
    if type(name) == "table" then
      for k,v in pairs(name) do
        msgc[k] = v
      end
      
      name = name.name
    end
    
    msgc.name = name
    self[name] = msgc
    local filename = debug.getinfo(2).source:split("/")
    print("Added new message: " .. name .. "@" .. filename[#filename])
  end,
  
  __index = function(t,k)
    return rawget(t, k) or L("setmetatable({}, {__index = function(t,k) return t end, __call = function(t) return t end}), print('Module not loaded: " .. k .. "')")
  end
})
msgs({name = "error", color = "${darkred}", keepers = {prefix = "", suffix = "${red}! ${reset}"}})

return msgs
