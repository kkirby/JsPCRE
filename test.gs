let Module = require('./pcre.js')

let regularExpression = """
	(?(DEFINE)
		(?<unit_types>em|ex|%|px|cm|mm|in|pt|pc|ch|rem|vh|vw|vmin|vmax|rad|deg)
	)
	^(?<count>(?:(?!(?&unit_types)).)+)(?<unit>(?&unit_types))\$
	"""
let matchString = '14px'

console.log 'testing pcre'
console.log new Module.RegExp(regularExpression,Module.PCRE_EXTENDED).match matchString

console.log ''

console.log 'testing internal'
console.log new RegExp(regularExpression).match matchString
