# DB2 Reader

This is a small library to read DB2 and ADB files (data tables) from World of Warcraft.

## Installation

Just copy the class file into your project and include it where you want to use it. Eventually I want to make this compatible with Composer, but that's not done yet.

## Usage

```php
// Instantiate an object with the path to a DB2 file.
$db2Reader = new Reader("Achievement.db2"); 
 
// Records are presented as simple arrays.
// Some fields are, themselves, arrays of values.
// Get individual records by ID with:
$record = $db2Reader->getRecord(17);
 
// Loop through records with:
foreach ($db2Reader->generateRecords() as $id => $record) { ... }
 
// All valid record IDs:
$ids = $db2Reader->getIds();
 
// You can set field names for convenience:
$db2Reader->setFieldNames([0 => "name", 1 => "note", ...]);
$record = $db2Reader->getRecord(17);
if ($record['name'] == "...") ...
 
// All integers are assumed to be unsigned, but you can change that by field:
$db2Reader->setFieldsSigned([2 => true, 7 => true]);
$record = $db2Reader->getRecord(17);
if ($record[2] < 0) ...
 
// You can get the calculated field types, 
// useful when dynamically creating database tables:
$types = $db2Reader->getFieldTypes();
if ($types['name'] == Reader::FIELD_TYPE_STRING) ...
 
// Finally, you can load an ADB, as long as you have its parent DB2.
// The ADB reader will only expose records in the ADB file
$adbReader = $db2Reader->loadAdb("Achievement.adb");
```

Also check out example.php, which is what I'm using during development.

## Compatibility

Works best with the WDB5 format, which is used in World of Warcraft: Legion. It should work well for most DB2 files, with support for embedded IDs, ID blocks, the copy block, and including Item-sparse with its embedded strings. Other files with embedded strings will need you to identify the string fields.

Warlords of Draenor (and Cataclysm, and Mists of Pandaria) uses the WDB2 format, which is also compatible with this library. Support for WDB2 is not as robust, as there are fewer clues regarding field types.

Legion alpha already went through WDB3 and WDB4, and I do not intend to support those versions, as the record structure for those formats is stored in the WoW executable.

ADB support is limited to WCH8, currently used in Legion. ADBs require their counterpart DB2 file for the necessary formatting.

## Goals

This is used for The Undermine Journal ([Newsstand](https://github.com/erorus/newsstand/)) to datamine items, pets, and other entities.

This is separate from the rest of Newsstand because I'd like to work on best practices with PSRs, unit tests, composer compatibility, etc. Eventually.

## Disclaimers

This work is neither endorsed by nor affiliated with Blizzard Entertainment.

This work is developed with no involvement of ZAM Network, which is my current employer.

## Thanks

Most of the file format details were found by documentation at [the WoWDev wiki](https://wowdev.wiki/DB2). Thanks to those who contribute there!

## License

Copyright 2016 Gerard Dombroski

Licensed under the Apache License, Version 2.0 (the "License");
you may not use these files except in compliance with the License.
You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

## Links
* [Github](https://github.com/erorus/db2)