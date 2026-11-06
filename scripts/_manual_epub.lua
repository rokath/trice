-- EPUB readers often lack emoji fonts. Use readable labels for editorial
-- symbols without changing UTF-8 examples, code, link targets or identifiers.
local labels = {
  ["ℹ"] = "[i]",
  ["💡"] = "[Tip]",
  ["❗"] = "[!]",
  ["⚠"] = "[Warning]",
  ["⛔"] = "[Unsupported]",
  ["🔍"] = "[Check]",
  ["✅"] = "[Yes]",
  ["❌"] = "[No]",
}

-- Str visits prose, table cells and heading/link labels, but never Code or
-- CodeBlock nodes. Consume a symbol's optional text/emoji variation selector
-- together with that symbol so no orphan selector becomes a missing glyph.
function Str(element)
  for symbol, label in pairs(labels) do
    element.text = element.text:gsub(symbol .. "️", label)
    element.text = element.text:gsub(symbol .. "︎", label)
    element.text = element.text:gsub(symbol, label)
  end
  return element
end

-- The symbol legend quotes these two editorial labels as inline code. They
-- are not executable examples; leave every other inline code value untouched.
function Code(element)
  if element.text == "✅ Yes" then
    element.text = "[Yes] Yes"
  elseif element.text == "❌ No" then
    element.text = "[No] No"
  end
  return element
end

-- Configuration tables need little space for their default value. Preserve
-- alignment while reserving most of the page for names and explanations.
function Table(element)
  if #element.colspecs == 3 and #element.head.rows == 1 then
    local cells = element.head.rows[1].cells
    if #cells == 3 and pandoc.utils.stringify(cells[2].contents) == "Default" then
      element.colspecs[1][2] = 0.44
      element.colspecs[2][2] = 0.12
      element.colspecs[3][2] = 0.44
    end
  end
  return element
end
