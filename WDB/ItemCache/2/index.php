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
if(isset($_GET['from']) && isset($_GET['to'])) {
  $search = "";
  $pagetitle = "Transformation, go!";
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
			$hex_decimal = convert_to_hex($item_icond);
			
			// Convert Hexadecimal to itemcache.wdb model editing string
			$itemcache_wdb = reverse_hex($hex_decimal);
			
			// Itemid decimal -> hex
			$hex_id_decimal = convert_to_hex($item_id);
			
			// Reverse the item id hex
			$itemcache_id_wdb = reverse_hex($hex_id_decimal);
			
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
			$item_quana = "<span style=\"color:gray\">Unavailable</span><br />";
			$item_class = "<span style=\"color:gray\">Unavailable</span>";
			$item_icond = "<span style=\"color:gray\">Unavailable</span>";
			$hex_decimal = "<span style=\"color:gray\">Unavailable</span>";
			$itemcache_wdb = "Unavailable";
			$hex_id_decimal = "<span style=\"color:gray\">Unavailable</span>";
			$itemcache_id_wdb = "<span style=\"color:gray\">Unavailable</span>";
			$sub_link = "<span style=\"color:red\">Error, the file you tried to view is not saved in the database!<br />           <a href=\"?p=cache\">Click here</a> to view the list of valid cache files that can be viewed.</span>";
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
<?php
if(strlen($item_name)==0 && strlen($item_id)==0) {
?>
<h1>Item not found!</h1>
</div>
<?php
} else {
?>
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
           
           <b>Display id:</b>      <?=$item_icond."\n";?>
           Hexadecimal:     <?=$hex_decimal."\n";?>
           Itemcache.wdb:   <span style="color:orange;cursor:help;" onclick="javascript:prompt('Ctrl+C or Right-click the selected text and Copy it','<?=$itemcache_wdb;?>')"><?=$itemcache_wdb;?></span> <span style="color:gray">(the display model in reverse hex, used for itemcache.wdb editing)</span>
           
           <b>Item id:</b>         <?=$item_id."\n";?>
           Hexadecimal:     <?=$hex_id_decimal."\n";?>
           Itemcache.wdb:   <span style="color:orange;cursor:help;" onclick="javascript:prompt('Ctrl+C or Right-click the selected text and Copy it','<?=$itemcache_id_wdb;?>')"><?=$itemcache_id_wdb;?></span> <span style="color:gray">(the item id of the item in reverse hex, used for looking up this itemin itemcache.wdb)</span>           
           <?=$sub_link;?>
<?php
}
?>
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

new Listview({template: 'item', id: 'items', name: LANG.tab_items, tabs: myTabs, parent: 'lkljbjkb574', note: sprintf(LANG.lvnote_itemsfound, 998, 200) + LANG.dash + LANG.lvnote_trynarrowing, _truncated: 1, data: [<?=$get_between_javascript_b;?>]});
var myTabs = new Tabs({parent: ge('jkbfksdbl4')});
myTabs.flush();
//]]></script>
<!-- Listing here: (but data above in the javascript - oddly not firefox) -->
<div class="listview-void"></div>
<div class="clear"></div>
</div>
</div>

<?php
} elseif(isset($_GET['from']) && isset($_GET['to'])) {

$trans_from_id = @$_GET['from'];
$trans_to_id = @$_GET['to'];

if(!preg_match("/^([0-9]{1,})$/", $trans_from_id)) {
  $trans_from_id = -1;
}
if(!preg_match("/^([0-9]{1,})$/", $trans_to_id)) {
  $trans_to_id = -1;
}

$out_code = "";
if($trans_from_id > -1 && $trans_to_id > -1) {
  $works_fine = true;
  
  // Read from cache or fresh from Wowhead then store in our cache
  $file_1_data = "";
  $file_2_data = "";
  $filenotcache1 = 0;
  $filenotcache2 = 0;
  $file1 = file_get_contents("dump/_{$trans_from_id}-dump.xml");
  $file2 = file_get_contents("dump/_{$trans_to_id}-dump.xml");
  if(!$file1) {
    $filenotcache1 = 1;
    $file1_get = file_get_contents("http://www.wowhead.com/?item={$trans_from_id}&xml");
    $file1_local = ".\dump\_{$trans_from_id}-dump.xml";
    if(!file_exists($file1_local)) {
      $fh1 = fopen($file1_local, "w+");
      fwrite($fh1, $file1_get);
      fclose($fh1);
    }
    $file_1_data = $file1_get;
  } else {
    $file_1_data = $file1;
  }
  if(!$file2) {
    $filenotcache2 = 1;
    $file2_get = file_get_contents("http://www.wowhead.com/?item={$trans_to_id}&xml");
    $file2_local = ".\dump\_{$trans_to_id}-dump.xml";
    if(!file_exists($file2_local)) {
      $fh2 = fopen($file2_local, "w+");
      fwrite($fh2, $file2_get);
      fclose($fh2);
    }
    $file_2_data = $file2_get;
  } else {
    $file_2_data = $file2;
  }
  
  $xml1 = new SimpleXMLElement($file_1_data);
  $item1_error  = $xml1->xpath('/wowhead/error');         $item1_error = $item1_error[0];
  $item1_id	    = $xml1->xpath('/wowhead/item');          $item1_id    = $item1_id[0]['id'];
  $item1_name   = $xml1->xpath('/wowhead/item/name');     $item1_name  = $item1_name[0];
  $item1_level  = $xml1->xpath('/wowhead/item/level');    $item1_level = $item1_level[0];
  $item1_quaid  = $xml1->xpath('/wowhead/item/quality');  $item1_quaid = $item1_quaid[0]['id'];
  $item1_quana  = $xml1->xpath('/wowhead/item/quality');  $item1_quana = $item1_quana[0];
  $item1_claid  = $xml1->xpath('/wowhead/item/class');    $item1_claid = $item1_claid[0]['id'];
  $item1_class  = $xml1->xpath('/wowhead/item/class');    $item1_class = $item1_class[0];
  $item1_sclid  = $xml1->xpath('/wowhead/item/subclass');	$item1_sclid = $item1_sclid[0]['id'];
  $item1_sclas  = $xml1->xpath('/wowhead/item/subclass');	$item1_sclas = $item1_sclas[0];
  $item1_icond  = $xml1->xpath('/wowhead/item/icon');     $item1_icond = $item1_icond[0]['displayId'];
  $item1_iconi  = $xml1->xpath('/wowhead/item/icon');     $item1_iconi = $item1_iconi[0];
  $item1_insld  = $xml1->xpath('/wowhead/item/inventorySlot');  $item1_insld = $item1_insld[0]['id'];
  $item1_inslo  = $xml1->xpath('/wowhead/item/inventorySlot');  $item1_inslo = $item1_inslo[0];
  $item1_toolt  = $xml1->xpath('/wowhead/item/htmlTooltip');    $item1_toolt = $item1_toolt[0];
  $item1_link   = $xml1->xpath('/wowhead/item/link');     $item1_link  = $item1_link[0];
  
  $xml2 = new SimpleXMLElement($file_2_data);
  $item2_error  = $xml2->xpath('/wowhead/error');         $item2_error = $item2_error[0];
  $item2_id	    = $xml2->xpath('/wowhead/item');          $item2_id    = $item2_id[0]['id'];
  $item2_name   = $xml2->xpath('/wowhead/item/name');     $item2_name  = $item2_name[0];
  $item2_level  = $xml2->xpath('/wowhead/item/level');    $item2_level = $item2_level[0];
  $item2_quaid  = $xml2->xpath('/wowhead/item/quality');  $item2_quaid = $item2_quaid[0]['id'];
  $item2_quana  = $xml2->xpath('/wowhead/item/quality');  $item2_quana = $item2_quana[0];
  $item2_claid  = $xml2->xpath('/wowhead/item/class');    $item2_claid = $item2_claid[0]['id'];
  $item2_class  = $xml2->xpath('/wowhead/item/class');    $item2_class = $item2_class[0];
  $item2_sclid  = $xml2->xpath('/wowhead/item/subclass');	$item2_sclid = $item2_sclid[0]['id'];
  $item2_sclas  = $xml2->xpath('/wowhead/item/subclass');	$item2_sclas = $item2_sclas[0];
  $item2_icond  = $xml2->xpath('/wowhead/item/icon');     $item2_icond = $item2_icond[0]['displayId'];
  $item2_iconi  = $xml2->xpath('/wowhead/item/icon');     $item2_iconi = $item2_iconi[0];
  $item2_insld  = $xml2->xpath('/wowhead/item/inventorySlot');  $item2_insld = $item2_insld[0]['id'];
  $item2_inslo  = $xml2->xpath('/wowhead/item/inventorySlot');  $item2_inslo = $item2_inslo[0];
  $item2_toolt  = $xml2->xpath('/wowhead/item/htmlTooltip');    $item2_toolt = $item2_toolt[0];
  $item2_link   = $xml2->xpath('/wowhead/item/link');     $item2_link  = $item2_link[0];
  
  if($item1_error == "Item not found!" || $item2_error == "Item not found!") {
    $error_out = "";
    if($item1_error == "Item not found!") {
      $goterr1 = true;
    }
    if($item2_error == "Item not found!") {
      $goterr2 = true;
    }
    if($goterr1 && !$goterr2) {
      $error_out .= " <em>(it's ".$trans_from_id." that is invalid)</em>";
    } elseif(!$goterr1 && $goterr2) {
      $error_out .= " <em>(it's ".$trans_to_id." that is invalid)</em>";
    } elseif($goterr1 && $goterr2) {
      $error_out .= " <em>(both ".$trans_from_id." and ".$trans_to_id." are invalid)</em>";
    }
    $out_code .= "This is odd! It seems like one of the items".$error_out." you query do not exist!<br /><br /><a href=\"./?from=".$trans_from_id."&to=".$trans_to_id."\">Click here</a> and try to query again, or <a href=\"./?from=-1&to=-1\">click here</a> to try with some other item id.";
  } else {

    
    $i1_idh = convert_to_hex($item1_id);
    $i1_idr = reverse_hex($i1_idh);
    $i1_dih = convert_to_hex($item1_icond);
    $i1_dir = reverse_hex($i1_dih);
    $i2_dih = convert_to_hex($item2_icond);
    $i2_dir = reverse_hex($i2_dih);
    
    $out_code .= "<b style=\"color:#FF3333;\">Before we start, shut down the game completely by exiting it!</b>
<ol class=\"transform\">
<li>Open &quot;itemcache.wdb&quot; in your favorite hex editor <em class=\"g\">(I use Hex Workshop)</em></li>
<li>Search for ".prompt($i1_idr)." and remember to select the first result from the result window <em class=\"g\">(CTRL+F, paste the hex, select &quot;Hex Values&quot; and &quot;Find All Instances&quot;)</em></li>
<li>Select &quot;Edit&quot; from the menu, then &quot;Goto...&quot; <em class=\"g\">(or CTRL+G)</em><br />
a. Enter ".prompt(20)." as offset (Dec) <em class=\"g\">(search from &quot;Current Position&quot;)</em></li>
<li>The selection should now be at the item name, if that is correct proceed to step #5<br />
a. If item name is not right, CTRL+F and search for &quot;Text String&quot;, look for ".prompt($item1_name)."<br />
b. Once you find the correct item, continue to step #5</li>
<li>Search for ".prompt($i1_dir).", you should now be right after the item name you just found! Again, remember to select the result from the result window below.</li>
<li>Replace ".$i1_dir." with ".prompt($i2_dir)." <em class=\"g\">(put the cursor on the front of ".$i1_dir." in the hex-view and then type ".$i2_dir.")</em></li>
<li>Save the file and start the game</li>
</ol>
<span class=\"c\">Congratulations! You have successfully replaced the model of ".wowheadilink($item1_name, $item1_id, $item1_quaid)." with ".wowheadilink($item2_name, $item2_id, $item2_quaid)." -enjoy!</span>\n";
    
  }
  
} else {
  $works_fine = false;
  $out_code .= "Make my <input type=\"text\" name=\"from\" class=\"itemid\" value=\"(item id)\" onfocus=\"if(this.value=='(item id)'){this.value='';}\" /> look like <input type=\"text\" name=\"to\" class=\"itemid\" value=\"(item id)\" onfocus=\"if(this.value=='(item id)'){this.value='';}\" />, <input type=\"submit\" class=\"itemid\" value=\"please\" />!";
}

?>


<div id="main">
<div id="main-precontents"></div>
<div id="main-contents" class="main-contents">
<?php
if($works_fine) {
?>
<div class="text"><h1>Transforming <em><?=$trans_from_id;?></em> to <em><?=$trans_to_id;?></em> in no-time!</h1></div>
<?php
} else {
?>
<div class="text"><h1>What do you want to transform today?</h1></div>
<?php
}
?>
<div id="jkbfksdbl4"></div>

<form method="GET" action="">
<div id="lkljbjkb574" class="listview" style="padding:12px;"><?=$out_code;?></div>
</form>

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
<div id="lkljbjkb574" class="listview"><br />
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
					#echo "&nbsp;&nbsp; <a href=\"?p=cache&nomercy=1\" style=\"color:red\">Flush</a> - <span style=\"color:gray\"><i>Parsing itemid \"{$bug}\" ended up with a fatal error!</i></span><br />\n";
					$bug = str_replace("_","",str_replace("-dump.xml","",$files[$i]));
					echo "<table border=\"0\" cellpadding=\"0\" style=\"border-collapse: collapse\" width=\"100%\" height=\"1\"><tr><td align=\"left\" width=\"35\">&nbsp;&nbsp; <a href=\"?p=cache&nomercy=1\" style=\"color:red\">Flush</a> - <span style=\"color:gray\"><i>Parsing itemid \"{$bug}\" ended up with a fatal error!</i></span></td></tr></table>\n";
				}
			}
		}
	}
}
?>
<br /></div>
<?php
} else {
?>
<div id="lkljbjkb574" class="listview"><br />&nbsp;&nbsp; To lookup the database for model hexadecimals, simply enter in a item name in the search field above.<br /><br />&nbsp;&nbsp; If Wowhead is currently unavailable you can <a href="?p=cache">browse the database cache files</a>, perhaps you will find what you like in there.<br /><br />
&nbsp;&nbsp;<span style="color:red">NEW!</span> If you want to swap a item model with something else, <a href="./?from=-1&to=-1">try out the &quot;walktrough&quot; tool</a>!
<br /><br /></div>
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
<div id="footer">Want the source for this tool? <a href="http://vlacula.no-ip.com/itemhex/nightly.zip">Click here</a> and download it (nightly builds)!</div>'."\n";
?>
<!-- piwik -->
<script src="http://vlacula.no-ip.com/phpmv3/piwik.js" type="text/javascript"></script>
<script type="text/javascript">
piwik_idsite = 1; //6
piwik_action_name = document.title;
piwik_url = "http://vlacula.no-ip.com/phpmv3/piwik.php";
piwik_log(piwik_action_name, piwik_idsite, piwik_url);
</script>
<object><noscript><img src="http://vlacula.no-ip.com/phpmv3/piwik.php?idsite=1" style="display:none" alt=""/></noscript></object>
<!-- /piwik -->
<?php
echo '</body>
</html>';

function convert_to_hex($str) {
  $out = dechex($str);
  if(strlen($out)==0) {
    $out = "0000";
  } elseif(strlen($out)==1) {
    $out = "000".dechex($str);
  } elseif(strlen($out)==2) {
    $out = "00".dechex($str);
  } elseif(strlen($out)==3) {
    $out = "0".dechex($str);
  } else {
    $out = dechex($str);
  }
  return $out;
}

function reverse_hex($hex) {
  if(strlen($hex)==0) {
    $hex1 = "00";
    $hex2 = "00";
  } elseif(strlen($hex)==1) {
    $hex1 = "00";
    $hex2 = "0".$hex;
  } elseif(strlen($hex)==2) {
    $hex1 = "00";
    $hex2 = $hex;
  } elseif(strlen($hex)==3) {
    $hex1 = substr($hex,2,1)."0";
    $hex2 = substr($hex,0,2);
  } elseif(strlen($hex)==4) {
    $hex1 = substr($hex,2,2);
    $hex2 = substr($hex,0,2);
  } else {
    $hex1 = "Not ";
    $hex2 = "supported (5 chars string)";
  }
  return $hex1.$hex2;
}

function prompt($str) {
  $str_js = $str;
  $str_js = str_replace("'", "\'", $str_js);
  $str_js = str_replace('"', "&quot;", $str_js);
  $str_li = $str;
  return "<span style=\"color:orange;cursor:help;\" onclick=\"javascript:prompt('Ctrl+C or Right-click the selected text and Copy it','".$str_js."')\">".$str_li."</span>";
}

function wowheadilink($n, $i, $q) {
  return "<a class=\"q".$q."\" style=\"text-decoration:none;\" href=\"http://www.wowhead.com/?item=".$i."\" target=\"_blank\">[".$n."]</a>";
}

?>
