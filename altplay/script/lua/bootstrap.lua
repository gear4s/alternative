--ease require"my.module"
package.path = "./script/lua/?.lua;" .. package.path

--lua 5.3 workaround
loadstring = loadstring or load

local startTime = os.time()
local char = string.char

local urandom, entropy = io.open("/dev/urandom"), 0
if not urandom then entropy = os.time()
else
  for _ = 1,7 do entropy = entropy * 0x10 + urandom:read(1):byte() end
  urandom:close()
end
math.randomseed(entropy % 0x7FFFFFFF)

--simple module loader
local loadd = {}
for _, v in ipairs(bot.listdir("script/lua/modules.d")) do
  local load = {v:match("^((%d+)%-.+%.[Ll][Uu][Aa])$")}
  if #load == 2 then
    load[2] = tonumber(load[2])
    table.insert(loadd, load)
  end
end

irc.color = {
  endwhite        = "\0030",
  black           = "\0031",
  darkblue        = "\0032",
  darkgreen       = "\0033",
  red             = "\0034",
  darkred         = "\0035",
  darkviolet      = "\0036",
  orange          = "\0037",
  yellow          = "\0038",
  lightgreen      = "\0039",
  cyan            = "\00310",
  lightcyan       = "\00311",
  blue            = "\00312",
  violet          = "\00313",
  darkgray        = "\00314",
  lightgray       = "\00315",
  reset           = "\003"
}
irc.control = {
  bold            = char(2),
  underline       = char(31)
}

--[[
    overload pcall with function that uses an error function that gives 
    more informative error messages.
]]

local function xpcall_error_handler(error_message)
    if type(error_message) ~= "table" then
        return {debug.traceback(error_message, 2)}
    else
        return error_message
    end
end

native_pcall = pcall

pcall = function(func, ...)
    local outer_arg = {...}
    return xpcall(function() return func(table.unpack(outer_arg)) end, xpcall_error_handler)
end

table.sort(loadd, function(a, b) if a[2] ~= b[2] then return a[2] < b[2] else return a[1] < b[1] end end)
for _, v in ipairs(loadd) do dofile("script/lua/modules.d/"..v[1]) end

bot.hook(396, function(msg)
  if msg.is_server_message then
    irc.join("#altplay.dev", "")
  end
end)