--[[
  
  Per-channel configuration
  
]]

require"json"

local open = io.open

local function read_file(path)
    local file = open(path, "rb") -- r read mode and b binary mode
    if not file then return nil end
    local content = file:read "*a" -- *a or *all reads the whole file
    file:close()
    return content
end

local writetofile = function(f,c)
  file = open("script/lua/chanconfig/"..f, "w")
  file:write(c)
  file:close()
end

local module = {}
local channels = {}

module.set = function(info,what,to)
  what = what:split(".")
  local c = channels[info.target]
  print(type(c))
  for k,v in pairs(what) do
    print(v)
    lv = v
    if type(c[v]) ~= "table" then break end
    c = c[v]
  end
  c[lv] = to
end

module.get = function(target,what)
  
end

local chanMt
chanMt = function(t,f)
  return setmetatable({__prototype = t}, {
    __index = function(t,k)
      print("get",k)
      return chanMt(rawget(t, "__prototype")[k])
    end,
    __newindex = function(t,k,v)
      print("set",k,v)
      rawset(t.__prototype, k, v)
      writetofile(f, t)
    end
  })
end
module.new = function(info)
  local newChan = {
    prefix = "",
    modules = {
      youtube = {
        enabled = true,
        search = true,
        random = true
      }
    }
  }
  channels[info.msg.target] = chanMt(newChan, info.msg.target .. ".json")
  writetofile(info.msg.target .. ".json", json.encode(newChan))
end

module.init = function()
  for _, v in ipairs(bot.listdir("script/lua/chanconfig")) do
    local chanfile, c = v:match("^((.+)%.[Jj][Ss][Oo][Nn])$")
    if chanfile then
      channels[c] = chanMt(json.decode(read_file("script/lua/chanconfig/" .. c .. ".json")), chanfile)
    end
  end
end

return module
