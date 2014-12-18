(function () {
  "use strict";
  var matchString, Module, regularExpression;
  Module = require("./pcre.js");
  regularExpression = "(?(DEFINE)\n\t(?<unit_types>em|ex|%|px|cm|mm|in|pt|pc|ch|rem|vh|vw|vmin|vmax|rad|deg)\n)\n^(?<count>(?:(?!(?&unit_types)).)+)(?<unit>(?&unit_types))$";
  matchString = "14px";
  console.log("testing pcre");
  console.log(new (Module.RegExp)(regularExpression, Module.PCRE_EXTENDED).match(matchString));
  console.log("");
  console.log("testing internal");
  console.log(new RegExp(regularExpression).match(matchString));
}.call(this));
