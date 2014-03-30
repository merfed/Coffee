<?php
include("config.php");

// Fix search query
if(!$_GET['s']) {
	$search = "";
	$pagetitle = "Shall we start?";
} else {
	$search = $_GET['s'];
	$pagetitle = "Search results for: {$search}";
}
if($_GET['p']=="cache") {
	$pagetitle = "Database cache";
}

// Get between strings
function get_string_between($string, $start, $end){
	$string = " ".$string;
	$ini = strpos($string,$start);
	if ($ini == 0) return "";
	$ini += strlen($start);   
	$len = strpos($string,$end,$ini) - $ini;
	return substr($string,$ini,$len);
}

// Get "?i=#" query
$kekid = $_GET['i'];

if($kekid) {
	#for($i=19160;$i<=19160;$i++) {
	for($i=$kekid;$i<=$kekid;$i++) {
		
		// Read from cache or fresh from Wowhead then store in our cache
		if($_GET['cache']=="1") {
			$file = file_get_contents("dump/_{$i}-dump.xml");
			if(!$file) {
				$filenotcache = 1;
			}
		} else {
			$file = file_get_contents("http://www.wowhead.com/?item=$i&xml");
			$file_local = ".\dump\_$i-dump.xml";
			if(!file_exists($file_local)) {
				$fh = fopen($file_local,"w+");
				fwrite($fh,$file);
				fclose($fh);
			}
		}
		
		// SimpleXML
		if(!$filenotcache==1) {
			$xml = new SimpleXMLElement($file);
			$item_id	= $xml->xpath('/wowhead/item');			$item_id	= $item_id[0]['id'];
			$item_name	= $xml->xpath('/wowhead/item/name');	$item_name	= $item_name[0];
			$item_level	= $xml->xpath('/wowhead/item/level');	$item_level	= $item_level[0];
			$item_quaid	= $xml->xpath('/wowhead/item/quality');	$item_quaid	= $item_quaid[0]['id'];
			$item_quana	= $xml->xpath('/wowhead/item/quality');	$item_quana	= $item_quana[0];
			$item_claid	= $xml->xpath('/wowhead/item/class');	$item_claid	= $item_claid[0]['id'];
			$item_class	= $xml->xpath('/wowhead/item/class');	$item_class	= $item_class[0];
			$item_sclid	= $xml->xpath('/wowhead/item/subclass');	$item_sclid	= $item_sclid[0]['id'];
			$item_sclas	= $xml->xpath('/wowhead/item/subclass');	$item_sclas	= $item_sclas[0];
			$item_icond	= $xml->xpath('/wowhead/item/icon');	$item_icond	= $item_icond[0]['displayId'];
			$item_iconi	= $xml->xpath('/wowhead/item/icon');	$item_iconi	= $item_iconi[0];
			$item_insld	= $xml->xpath('/wowhead/item/inventorySlot');	$item_insld = $item_insld[0]['id'];
			$item_inslo	= $xml->xpath('/wowhead/item/inventorySlot');	$item_inslo = $item_inslo[0];
			$item_toolt	= $xml->xpath('/wowhead/item/htmlTooltip');	$item_toolt = $item_toolt[0];
			$item_link	= $xml->xpath('/wowhead/item/link');	$item_link = $item_link[0];
			
			// Fix Tooltip link HTML
			$item_toolt = str_replace('"',"'",$item_toolt);
			
			// Fix icond for url usage (wowhead is case sensitive!)
			$item_iconi_old = strtolower($item_iconi);
			$item_iconi = strtolower($item_iconi);
			$icon_url = "http://static.wowhead.com/images/icons/large/{$item_iconi}.jpg";
			
			// Decimal -> Hexadecimal convert
			$hex_decimal = dechex($item_icond);
			if(strlen($hex_decimal)==0) {
				$hex_decimal = "0000";
			} elseif(strlen($hex_decimal)==1) {
				$hex_decimal = dechex($item_icond)."000";
			} elseif(strlen($hex_decimal)==2) {
				$hex_decimal = dechex($item_icond)."00";
			} elseif(strlen($hex_decimal)==3) {
				$hex_decimal = dechex($item_icond)."0";
			} else {
				$hex_decimal = dechex($item_icond);
			}
			
			// Convert Hexadecimal to itemcache.wdb model editing string
			if(strlen($hex_decimal)==0) {
				$hex1 = "00";
				$hex2 = "00";
			} elseif(strlen($hex_decimal)==1) {
				$hex1 = "00";
				$hex2 = "0".$hex_decimal;
			} elseif(strlen($hex_decimal)==2) {
				$hex1 = "00";
				$hex2 = $hex_decimal;
			} elseif(strlen($hex_decimal)==3) {
				$hex1 = substr($hex_decimal,2,1)."0";
				$hex2 = substr($hex_decimal,0,2);
			} elseif(strlen($hex_decimal)==4) {
				$hex1 = substr($hex_decimal,2,2);
				$hex2 = substr($hex_decimal,0,2);
			} else {
				$hex1 = "Not ";
				$hex2 = "supported (5 chars string)";
			}
			
			// Output line for the itemcache.wdb hexadecimal data
			$itemcache_wdb = $hex1.$hex2;
			
			/*
			echo "<pre>
			Convert display id:\t<b>$display_id</b>
			
			Into hexadecimal:\t<b>$hex_decimal</b>
			
			Into itemcache.wdb:\t<b>$itemcache_wdb</b>
			</pre>";
			*/
			
			// Fix the empty "Slot:" entries and others
			if(strlen($item_inslo)==0) {
				#$slot_html = "\n";
				$slot_html = "  <span style='color:gray'>Slot:            <i>Unspecified</i></span>\n";
			} else {
				$slot_html = "  Slot:            $item_inslo\n";
			}
			if(strlen($item_sclas)==0) {
				#$subclass_html = "\n";
				$subclass_html = "           <span style='color:gray'>Sub class:       <i>None</i></span>\n";
			} else {
				$subclass_html = "           Sub class:       $item_sclas\n";
			}
			if(!(	$item_class=="Quest" or 
					$item_class=="Trade Goods" or 
					$item_sclas=="Miscellaneous" or 
					$item_sclas=="Mounts" or 
					$item_sclas=="Junk" 
				)) {
				/*
				$sub_link = "<div style=\"position:relative;left:88px;top:6px\" id=\"flashmodel\"></div><script>
var fo = new SWFObject(\"http://www.wowhead.com/modelviewer2/viewer2R1.swf\",\"tgkejfh464\",\"600\",\"400\",7,\"#181818\");
fo.addParam(\"quality\",\"low\");
fo.addParam(\"FlashVars\",\"model=http://www.wowhead.com/modelviewer2/models/item/{$item_icond}.dae&link=http://www.wowhead.com/?weapons={$item_claid}&logo=wowhead\");
fo.write(\"flashmodel\");
</script>";
				*/
			}
			$pagetitle = " {$item_name}";
		} else {
			$pagetitle = "File not found";
			$item_name = "\"File not found\"";
			$item_id = "unavailable";
			$item_level = "<span style=\"color:gray\">Unavailable</span>";
			$item_quana = "<span style=\"color:gray\">Unavailable</span><br>";
			$item_class = "<span style=\"color:gray\">Unavailable</span>";
			$item_icond = "<span style=\"color:gray\">Unavailable</span>";
			$hex_decimal = "<span style=\"color:gray\">Unavailable</span>";
			$itemcache_wdb = "Unavailable";
			$sub_link = "<span style=\"color:red\">Error, the file you tried to view is not saved in the database!<br>           <a href=\"?p=cache\">Click here</a> to view the list of valid cache files that can be viewed.</span>";
		}
		
	}
}

// HTML
echo '<html>
<head>
<title>Itemhex: '.$pagetitle.'</title>
<link rel="stylesheet" type="text/css" href="global.css" /><!--[if IE]>
<link rel="stylesheet" type="text/css" href="global_ie.css" /><![endif]--><!--[if lte IE 6]>
<link rel="stylesheet" type="text/css" href="global_ie6.css" /><![endif]-->
<script src="config.php?js=1" type="text/javascript"></script>
<script src="locale_enus.js" type="text/javascript"></script>
<script src="global.js" type="text/javascript"></script>
<script src="swfobject.js" type="text/javascript"></script>
<script src="http://www.wowhead.com/widgets/power.js" type="text/javascript"></script>
</head>
<body>
<div id="layers"></div>
<div id="layout">
<div id="header">
<div id="header-logo"><a href="./"></a></div>
</div>
<div id="wrapper" style="width:100%;text-align:left;vertical-align:top">
<div id="toptabs">
<div id="toptabs-inner">
<div id="toptabs-right"></div>
<div id="ptewhjkst46"></div>
<div class="clear"></div>
</div>
</div>
<div id="topbar-right"><div><form action="./" method="get"><a href="javascript:;" onclick="javascript:window.location=\'./?s=\'+document.getElementById(\'oh2345v5ks\').value;"></a><input style="height:22px" name="s" size="30" value="'.$search.'" id="oh2345v5ks" /></form></div></div>
<div id="topbar"><span id="kbl34h6b43" class="menu-buttons"></span><div class="clear"></div></div>';

if($_GET['i']) {
?>


<div id="main">
<div id="main-precontents"></div>
<div id="main-contents" class="main-contents">
<div class="text">
<h1>Showing data for <i><?=$item_name;?> (itemid <?=$item_id;?>)</i></h1>
</div>
<div id="jkbfksdbl4"></div>
<div id="lkljbjkb574" class="listview"><pre>
<div id="ic<?=$item_id;?>" style="float: left; position: relative; left: 10px;"></div>  Item name:       <span class="q<?=$item_quaid;?>"><?=$item_name;?></span> - <a href="<?=$item_link;?>" target="_<?=$item_id;?>">View Wowhead entry?</a>
  Item level:      <?=$item_level."\n";?>
  Quality:         <?=$item_quana."\n";?>
<?=$slot_html;?>
<div style="position: relative; left: -3px;">
           Class:           <?=$item_class."\n";?>
<?=$subclass_html;?>
           
           Display id:      <?=$item_icond."\n";?>
           Hexadecimal:     <?=$hex_decimal."\n";?>
           Itemcache.wdb:   <span style="color:orange;cursor:help;" onclick="javascript:prompt('Ctrl+C or Right-click the selected text and Copy it','<?=$itemcache_wdb;?>')"><?=$itemcache_wdb;?></span> <span style="color:gray">(Used for itemcache.wdb editing, the one above is raw)</span>
           
           <?=$sub_link;?>
</div>
</pre></div>
<script type="text/javascript">
ge('ic<?=$item_id;?>').appendChild(Icon.create('<?=$item_iconi_old;?>', 2, 0, 0, 1));
</script>
<div class="listview-void"></div>
<div class="clear"></div>
</div>
</div>

<?php
} elseif($search) {

// Query
$query = str_replace(" ","+",$search);
$search_query_url = "http://www.wowhead.com/?search=$query";
$search_html = file_get_contents($search_query_url);

// Part 1
$get_between_javascript_a = get_string_between($search_html, "var _ = g_items;", "var _ = g_spells");
if(strpos("]",$get_between_javascript_a)==false) {
	$get_between_javascript_a = get_string_between($search_html, "var _ = g_items;", "var myTabs = ");
}

// Part 2
$get_between_javascript_b = get_string_between($search_html, "', data: [", "]});");
if(preg_match('/react:/', $get_between_javascript_b)) {
	$get_between_javascript_b = "";
}

// Auto-redirect
$get_between_check = get_string_between($search_html, ", typeId: ", ", name: '");
if(strlen($get_between_check)>0 and strlen($get_between_check)<6) {
	#header("location: ./?i=$get_between_check");
	echo "<script>window.location='./?i=$get_between_check'</script>";
}

// Debug
#echo "<textarea>$get_between_javascript_b</textarea>";
#echo "<textarea>$get_between_check</textarea>";
#echo "<textarea>$search_html</textarea>";
?>


<div id="main">
<div id="main-precontents"></div>
<div id="main-contents" class="main-contents">
<div class="text">
<h1>Search results for <i><?=$search;?></i></h1>
</div>
<div id="jkbfksdbl4"></div>
<div id="lkljbjkb574" class="listview"></div>
<script type="text/javascript">//<![CDATA[
var _ = g_items;
<?=$get_between_javascript_a;?>


new Listview({template: 'item', id: 'items', name: 'Items', tabs: myTabs, parent: 'lkljbjkb574', data: [<?=$get_between_javascript_b;?>]});
var myTabs = new Tabs({parent: ge('jkbfksdbl4')});
myTabs.flush();

//]]></script>
<!-- Listing here: (but data above in the javascript - oddly not firefox) -->
<div class="listview-void"></div>
<div class="clear"></div>
</div>
</div>

<?php
} else {
?>


<div id="main">
<div id="main-precontents"></div>
<div id="main-contents" class="main-contents">
<div class="text">
<?php
if($_GET['p']=="cache") {
?>
<h1>Database cache</h1>
<?php
} else {
?>
<h1>Let's begin..</h1>
<?php
}
?>
</div>
<div id="jkbfksdbl4"></div>
<?php
if($_GET['p']=="cache") {
?>
<div id="lkljbjkb574" class="listview"><br>
<?php
#echo "&nbsp;&nbsp; Cache";
$dumpdir = opendir("dump/");
while($file = readdir($dumpdir)) {
	$files[] = $file;
}
$filecount = count($files);
closedir($dumpdir);
sort($files);
$count = 1;

for($i=0; $i < $filecount; $i++) {
	if (substr("$files[$i]",0,1) != ".") {
		$count = $count + 1;
		$file = file_get_contents("./dump/".$files[$i]);
		if(simplexml_load_string($file)) {
			$xml2 = new SimpleXMLElement($file);
			$item_error		= $xml2->xpath('/wowhead/error');								$item_error		= $item_error[0];
			$item_id2		= $xml2->xpath('/wowhead/item');									$item_id2			= $item_id2[0]['id'];
			$item_name2		= $xml2->xpath('/wowhead/item/name');						$item_name2		= $item_name2[0];
			$item_level2	= $xml2->xpath('/wowhead/item/level');					$item_level2	= $item_level2[0];
			$item_quaid2	= $xml2->xpath('/wowhead/item/quality');				$item_quaid2	= $item_quaid2[0]['id'];
			$item_quana2	= $xml2->xpath('/wowhead/item/quality');				$item_quana2	= $item_quana2[0];
			$item_claid2	= $xml2->xpath('/wowhead/item/class');					$item_claid2	= $item_claid2[0]['id'];
			$item_class2	= $xml2->xpath('/wowhead/item/class');					$item_class2	= $item_class2[0];
			$item_sclid2	= $xml2->xpath('/wowhead/item/subclass');				$item_sclid2	= $item_sclid2[0]['id'];
			$item_sclas2	= $xml2->xpath('/wowhead/item/subclass');				$item_sclas2	= $item_sclas2[0];
			$item_icond2	= $xml2->xpath('/wowhead/item/icon');						$item_icond2	= $item_icond2[0]['displayId'];
			$item_iconi2	= $xml2->xpath('/wowhead/item/icon');						$item_iconi2	= $item_iconi2[0];
			$item_insld2	= $xml2->xpath('/wowhead/item/inventorySlot');	$item_insld2 	= $item_insld2[0]['id'];
			$item_inslo2	= $xml2->xpath('/wowhead/item/inventorySlot');	$item_inslo2 	= $item_inslo2[0];
			$item_toolt2	= $xml2->xpath('/wowhead/item/htmlTooltip');		$item_toolt2 	= $item_toolt2[0];
			$item_link2		= $xml2->xpath('/wowhead/item/link');						$item_link2 	= $item_link2[0];
			if(strlen($item_id2)>0 and strlen($item_name2)>0 and !$item_error) {
				echo "<table border=\"0\" cellpadding=\"0\" style=\"border-collapse: collapse\" width=\"100%\" height=\"1\"><tr><td align=\"left\" width=\"35\">&nbsp; <img width=\"18\" height=\"18\" src=\"http://static.wowhead.com/images/icons/small/".strtolower($item_iconi2).".jpg\" border=\"0\"></td><td align=\"left\" width=\"50\"><small>{$item_id2}</small></td><td align=\"left\" width=\"300\"><span class=\"q{$item_quaid2}\"><a href=\"?i={$item_id2}&cache=1\" style=\"text-decoration:none\">{$item_name2}</a></span></td><td align=\"left\" width=\"50\">{$item_level2}</td><td align=\"left\" width=\"120\">{$item_class2}</td><td align=\"left\" width=\"180\">{$item_sclas2}</td><td align=\"left\">{$item_inslo2}</td><td align=\"left\">&nbsp;</td><td align=\"left\">&nbsp;</td></tr></table>\n";
			} else {
				if($_GET['nomercy']=="1") {
					unlink("./dump/".$files[$i]);
				} else {
					$bug = str_replace("_","",str_replace("-dump.xml","",$files[$i]));
					echo "<table border=\"0\" cellpadding=\"0\" style=\"border-collapse: collapse\" width=\"100%\" height=\"1\"><tr><td align=\"left\" width=\"35\">&nbsp;&nbsp; <a href=\"?p=cache&nomercy=1\" style=\"color:red\">Flush</a> - <span style=\"color:gray\"><i>Parsing itemid \"{$bug}\" ended up with a fatal error!</i></span></td></tr></table>\n";
				}
			}
		} else {
			if($_GET['nomercy']=="1") {
				if($files[$i]!="index.php") {
					unlink("./dump/".$files[$i]);
				}
			} else {
				if($files[$i]!="index.php") {
					#echo "&nbsp;&nbsp; <a href=\"?p=cache&nomercy=1\" style=\"color:red\">Flush</a> - <span style=\"color:gray\"><i>Parsing itemid \"{$bug}\" ended up with a fatal error!</i></span><br>\n";
					$bug = str_replace("_","",str_replace("-dump.xml","",$files[$i]));
					echo "<table border=\"0\" cellpadding=\"0\" style=\"border-collapse: collapse\" width=\"100%\" height=\"1\"><tr><td align=\"left\" width=\"35\">&nbsp;&nbsp; <a href=\"?p=cache&nomercy=1\" style=\"color:red\">Flush</a> - <span style=\"color:gray\"><i>Parsing itemid \"{$bug}\" ended up with a fatal error!</i></span></td></tr></table>\n";
				}
			}
		}
	}
}
?>
<br></div>
<?php
} else {
?>
<div id="lkljbjkb574" class="listview"><br>&nbsp;&nbsp; To lookup the database for model hexadecimals, simply enter in a item name in the search field above.<br><br>&nbsp;&nbsp; If Wowhead is currently unavailable you can <a href="?p=cache">browse the database cache files</a>, perhaps you will find what you like in there.<br><br></div>
<?php
}
?>
<div class="listview-void"></div>
<div class="clear"></div>
</div>
</div>

<?php
}

echo '</div>
</div>
<div id="footer"><a href="http://shhh.heylove.org/">Essentia</a>!</div>
</body>
</html>';

?>
