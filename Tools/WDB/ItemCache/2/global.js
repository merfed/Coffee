function $(C) {
   if(arguments.length > 1) {
      var D = [];
      for(var B = 0, A = arguments.length; B < A; ++B) {
         D.push($(arguments[B]))}
      return D}
   if(typeof C == "string") {
      C = ge(C)}
   return C}
function $E(A) {
   if(!A) {
      if(typeof event != "undefined") {
         A = event}
      else {
         return null}
      }
   if(A.which) {
      A._button = A.which}
   else {
      A._button = A.button;
      if(Browser.ie) {
         if(A._button & 4) {
            A._button = 2}
         else {
            if(A._button & 2) {
               A._button = 3}
            }
         }
      else {
         A._button = A.button + 1}
      }
   A._target = A.target ? A.target : A.srcElement;
   return A}
function $A(B) {
   var D = [];
   for(var C = 0, A = B.length; C < A; ++C) {
      D.push(B[C])}
   return D}
Function.prototype.bind = function() {
   var A = this, C = $A(arguments), B = C.shift();
   return function() {
      return A.apply(B, C.concat($A(arguments)))}
   };
function strcmp(B, A) {
   if(B == A) {
      return 0}
   if(B == null) {
      return - 1}
   if(A == null) {
      return 1}
   return B < A ?- 1 : 1}
function trim(A) {
   return A.replace(/(^\s*|\s*$)/g,"")}function rtrim(B,C){var A=B.length;
   while(--A > 0 && B.charAt(A) == C) {
      }
   B = B.substring(0, A + 1);
   if(B == C) {
      B = ""}
   return B}
function sprintf(B) {
   var A;
   for(A = 1, len = arguments.length; A < len; ++A) {
      B = B.replace("$" + A, arguments[A])}
   return B}
function str_replace(C, B, A) {
   while(C.indexOf(B) !=- 1) {
      C = C.replace(B, A)}
   return C}
function urlencode(A) {
   A = encodeURIComponent(A);
   A = str_replace(A, "+", "%2B");
   return A}
function number_format(A) {
   A = "" + parseInt(A);
   if(A.length <= 3) {
      return A}
   return number_format(A.substr(0, A.length - 3)) + "," + A.substr(A.length - 3)}
function in_array(B, E, F, D) {
   if(B == null) {
      return - 1}
   if(F) {
      return in_arrayf(B, E, F, D)}
   for(var C = D || 0, A = B.length; C < A; ++C) {
      if(B[C] == E) {
         return C}
      }
   return - 1}
function in_arrayf(B, E, F, D) {
   for(var C = D || 0, A = B.length; C < A; ++C) {
      if(F(B[C]) == E) {
         return C}
      }
   return - 1}
function array_walk(C, F, B) {
   var E;
   for(var D = 0, A = C.length; D < A; ++D) {
      E = F(C[D], B, C, D);
      if(E != null) {
         C[D] = E}
      }
   }
function array_apply(C, F, B) {
   var E;
   for(var D = 0, A = C.length; D < A; ++D) {
      F(C[D], B, C, D)}
   }
function ge(A) {
   return document.getElementById(A)}
function gE(A, B) {
   return A.getElementsByTagName(B)}
function ce(C, B) {
   var A = document.createElement(C);
   if(B) {
      cOr(A, B)}
   return A}
function de(A) {
   A.parentNode.removeChild(A)}
function ae(A, B) {
   return A.appendChild(B)}
function ct(A) {
   return document.createTextNode(A)}
function nw(A) {
   A.style.whiteSpace = "nowrap"}
function rf() {
   return false}
function rf2(A) {
   A = $E(A);
   if(A.ctrlKey || A.shiftKey || A.altKey || A.metaKey) {
      return }
   return false}
function tb() {
   this.blur()}
function ac(C) {
   var B = 0, D = 0;
   while(C) {
      B += C.offsetLeft;
      D += C.offsetTop;
      C = C.offsetParent}
   var A = [B, D];
   A.x = B;
   A.y = D;
   return A}
function aE(B, C, A) {
   if(Browser.ie) {
      B.attachEvent("on" + C, A)}
   else {
      B.addEventListener(C, A, false)}
   }
function dE(B, C, A) {
   if(Browser.ie) {
      B.detachEvent("on" + C, A)}
   else {
      B.removeEventListener(C, A, false)}
   }
function sp(A) {
   if(!A) {
      A = event}
   if(Browser.ie) {
      A.cancelBubble = true}
   else {
      A.stopPropagation()}
   }
function sc(F, G, B, D, E) {
   var C = new Date();
   var A = F + "=" + escape(B) + "; ";
   C.setDate(C.getDate() + G);
   A += "expires=" + C.toUTCString() + "; ";
   if(D) {
      A += "path=" + D + "; "}
   if(E) {
      A += "domain=" + E + "; "}
   document.cookie = A;
   gc.C[F] = B}
function dc(A) {
   sc(A, - 1);
   gc.C[A] = null}
function gc(F) {
   if(gc.I == null) {
      var E = unescape(document.cookie).split("; ");
      for(var C = 0, A = E.length; C < A; ++C) {
         var G = E[C].indexOf("="), B, D;
         if(G !=- 1) {
            B = E[C].substr(0, G);
            D = E[C].substr(G + 1)}
         else {
            B = E[C];
            D = ""}
         gc.C[B] = D}
      gc.I = 1}
   if(!F) {
      return gc.C}
   else {
      return gc.C[F]}
   }
gc.C = {
   };
function ns(A) {
   if(Browser.ie) {
      A.onfocus = tb;
      A.onmousedown = A.onselectstart = A.ondragstart = rf}
   }
function cO(C, A) {
   for(var B in A) {
      C[B] = A[B]}
   }
function cOr(C, A) {
   for(var B in A) {
      if(typeof A[B] == "object") {
         if(!C[B]) {
            C[B] = {
               }
            }
         cOr(C[B], A[B])}
      else {
         C[B] = A[B]}
      }
   }
var Browser = {
   ie :!!(window.attachEvent &&!window.opera), opera :!!window.opera, safari : navigator.userAgent.indexOf("Safari") !=- 1, gecko : navigator.userAgent.indexOf("Gecko") !=- 1 && navigator.userAgent.indexOf("KHTML") ==- 1};
Browser.ie7 = Browser.ie && navigator.userAgent.indexOf("MSIE 7.0") !=- 1;
Browser.ie6 = Browser.ie && navigator.userAgent.indexOf("MSIE 6.0") !=- 1 &&!Browser.ie7;
navigator.userAgent.match(/Gecko\/([0-9]+)/);
Browser.geckoVersion = parseInt(RegExp.$1) | 0;
var OS = {
   windows : navigator.appVersion.indexOf("Windows") !=- 1, mac : navigator.appVersion.indexOf("Macintosh") !=- 1, linux : navigator.appVersion.indexOf("Linux") !=- 1};
var DomContentLoaded = new function() {
   var A = [], B;
   this.now = function() {
      if(B) {
         return }
      array_apply(A, function(C) {
         C()}
      );
      DomContentLoaded = null};
   this.addEvent = function(C) {
      A.push(C)}
   };
function g_getWindowSize() {
   var B = 0, A = 0;
   if(typeof window.innerWidth == "number") {
      B = window.innerWidth;
      A = window.innerHeight}
   else {
      if(document.documentElement && (document.documentElement.clientWidth || document.documentElement.clientHeight)) {
         B = document.documentElement.clientWidth;
         A = document.documentElement.clientHeight}
      else {
         if(document.body && (document.body.clientWidth || document.body.clientHeight)) {
            B = document.body.clientWidth;
            A = document.body.clientHeight}
         }
      }
   return {
      w : B, h : A}
   }
function g_getScroll() {
   var A = 0, B = 0;
   if(typeof (window.pageYOffset) == "number") {
      A = window.pageXOffset;
      B = window.pageYOffset}
   else {
      if(document.body && (document.body.scrollLeft || document.body.scrollTop)) {
         A = document.body.scrollLeft;
         B = document.body.scrollTop}
      else {
         if(document.documentElement && (document.documentElement.scrollLeft || document.documentElement.scrollTop)) {
            A = document.documentElement.scrollLeft;
            B = document.documentElement.scrollTop}
         }
      }
   return {
      x : A, y : B}
   }
function g_getCursorPos(C) {
   var B, D;
   if(window.innerHeight) {
      B = C.pageX;
      D = C.pageY}
   else {
      var A = g_getScroll();
      B = C.clientX + A.x;
      D = C.clientY + A.y}
   return {
      x : B, y : D}
   }
function g_scrollTo(C, B) {
   var M, K = g_getWindowSize(), L = g_getScroll(), I = K.w, E = K.h, G = L.x, D = L.y;
   C = $(C);
   if(B == null) {
      B = []}
   else {
      if(typeof B == "number") {
         B = [B]}
      }
   M = B.length;
   if(M == 0) {
      B[0] = B[1] = B[2] = B[3] = 0}
   else {
      if(M == 1) {
         B[1] = B[2] = B[3] = B[0]}
      else {
         if(M == 2) {
            B[2] = B[0];
            B[3] = B[1]}
         else {
            if(M == 3) {
               B[3] = B[1]}
            }
         }
      }
   M = ac(C);
   var A = M[0] - B[3];
   var H = M[1] - B[0];
   var J = M[0] + C.offsetWidth + B[1];
   var F = M[1] + C.offsetHeight + B[2];
   if(J - A > I || A < G) {
      G = A}
   else {
      if(J - I > G) {
         G = J - I}
      }
   if(F - H > E || H < D) {
      D = H}
   else {
      if(F - E > D) {
         D = F - E}
      }
   scrollTo(G, D)}
function g_setTextNodes(C, B) {
   if(C.nodeType == 3) {
      C.nodeValue = B}
   else {
      for(var A = 0; A < C.childNodes.length; ++A) {
         g_setTextNodes(C.childNodes[A], B)}
      }
   }
function g_getTextContent(C) {
   var A = "";
   for(var B = 0; B < C.childNodes.length; ++B) {
      if(C.childNodes[B].nodeValue) {
         A += C.childNodes[B].nodeValue}
      else {
         if(C.childNodes[B].nodeName == "BR") {
            if(Browser.ie) {
               A += "\r"}
            else {
               A += "\n"}
            }
         }
      A += g_getTextContent(C.childNodes[B])}
   return A}
function g_setSelectedLink(C, B) {
   if(!g_setSelectedLink.groups) {
      g_setSelectedLink.groups = {
         }
      }
   var A = g_setSelectedLink.groups;
   if(A[B]) {
      A[B].className = ""}
   C.className = "selected";
   A[B] = C}
function g_toggleDisplay(A) {
   if(A.style.display == "none") {
      A.style.display = "";
      return true}
   else {
      A.style.display = "none";
      return false}
   }
function g_disclose(A, B) {
   B.className = "disclosure-" + (g_toggleDisplay(A) ? "on" : "off");
   return false}
function g_enableScrollEvent(A) {
   if(A.stopPropagation) {
      A.stopPropagation()}
   if(A.preventDefault) {
      A.preventDefault()}
   A.returnValue = false;
   A.cancelBubble = true;
   return false}
function g_enableScroll(A) {
   if(!A) {
      aE(document, "mousewheel", g_enableScrollEvent);
      aE(window, "DOMMouseScroll", g_enableScrollEvent)}
   else {
      dE(document, "mousewheel", g_enableScrollEvent);
      dE(window, "DOMMouseScroll", g_enableScrollEvent)}
   }
function g_getGets() {
   var E = {
      };
   if(location.search) {
      var F = decodeURIComponent(location.search.substr(1)).split("&");
      for(var C = 0, A = F.length; C < A; ++C) {
         var G = F[C].indexOf("="), B, D;
         if(G !=- 1) {
            B = F[C].substr(0, G);
            D = F[C].substr(G + 1)}
         else {
            B = F[C];
            D = ""}
         E[B] = D}
      }
   return E}
function g_initHeader(B) {
   var H = ce("dl");
   for(var G = 0, J = mn_path.length; G < J; ++G) {
      var E = ce("dt");
      var L = ce("a");
      var I = ce("ins");
      var F = ce("big");
      var D = ce("span");
      if(mn_path[G][0] != B && mn_path[G][3]) {
         L.menu = mn_path[G][3];
         L.onmouseover = Menu.show;
         L.onmouseout = Menu.hide}
      else {
         L.onmouseover = Menu._hide}
      if(mn_path[G][2]) {
         L.href = mn_path[G][2]}
      else {
         L.href = "javascript:;";
         ns(L);
         L.style.cursor = "default"}
      if(B != null && mn_path[G][0] == B) {
         L.className = "selected"}
      ae(F, ct(mn_path[G][1].charAt(0)));
      ae(I, F);
      ae(I, ct(mn_path[G][1].substr(1)));
      ae(L, I);
      ae(L, D);
      ae(E, L);
      ae(H, E)}
   ae(ge("ptewhjkst46"), H);
   var A = ge("kbl34h6b43");
   if(B != null && B >= 0 && B < mn_path.length) {
      switch(B) {
         case 0 : Menu.addButtons(A, Menu.explode(mn_database));
         break;
         case 1 : Menu.addButtons(A, mn_tools);
         break;
         case 2 : Menu.addButtons(A, Menu.explode(mn_more));
         break;
         case 3 : Menu.addButtons(A, Menu.explode(mn_forums));
         break}
      }
   else {
      ae(A, ct(String.fromCharCode(160)))}
   g_initLanguageChanger();
   var M = ge("oh2345v5ks");
   var K = M.previousSibling;
   var C = M.parentNode;
   ns(K);
   K.onclick = function() {
      this.parentNode.onsubmit()};
   if(Browser.ie) {
      setTimeout(function() {
         M.value = ""}
      , 1)}
   if(M.value == "") {
      M.className = "search-database"}
   M.onmouseover = function() {
      if(trim(this.value) != "") {
         this.className = ""}
      };
   M.onfocus = function() {
      this.className = ""};
   M.onblur = function() {
      if(trim(this.value) == "") {
         this.className = "search-database";
         this.value = ""}
      };
   C.onsubmit = function() {
      var N = this.elements[0].value;
      if(trim(N) == "") {
         return false}
      this.submit()}
   }
function g_initLanguageChanger() {
   var A = ge("language-changer");
   if(!A) {
      return }
   var C = "www", B = location.href, D;
   D = location.hostname.indexOf(".");
   if(D !=- 1 && D <= 5) {
      C = location.hostname.substr(0, D)}
   D = B.indexOf("#");
   if(D !=- 1) {
      B = B.substr(0, D)}
   A.menu = [[0, "Deutsch", (g_locale.id != 3 ? B.replace(C, "de") : null)], [0, "English", (g_locale.id != 0 ? B.replace(C, "www") : null)], [0, "Espa" + String.fromCharCode(241) + "ol", (g_locale.id != 6 ? B.replace(C, "es") : null)], [0, "Fran" + String.fromCharCode(231) + "ais", (g_locale.id != 2 ? B.replace(C, "fr") : null)], [0, String.fromCharCode(1056, 1091, 1089, 1089, 1082, 1080, 1081), (g_locale.id != 7 ? B.replace(C, "ru") : null)]];
   A.menu.rightAligned = 1;
   if(g_locale.id != 25) {
      A.menu[ {
         0 : 1, 2 : 3, 3 : 0, 6 : 2, 7 : 4}
      [g_locale.id]].checked = 1}
   A.onmouseover = Menu.show;
   A.onmouseout = Menu.hide}
function g_initPath(K, D) {
   var G = mn_path, H = null, C = null, L = 0, I = ge("main-precontents"), A = ce("div");
   A.className = "path";
   if(D != null) {
      var J = ce("div");
      J.className = "path-right";
      var M = ce("a");
      M.href = "javascript:;";
      M.id = "fi_toggle";
      ns(M);
      M.onclick = fi_Toggle;
      if(D) {
         M.className = "disclosure-on";
         ae(M, ct(LANG.fihide))}
      else {
         M.className = "disclosure-off";
         ae(M, ct(LANG.fishow))}
      ae(J, M);
      ae(I, J)}
   for(var F = 0; F < K.length; ++F) {
      var M, B, N = 0;
      for(var E = 0; E < G.length; ++E) {
         if(G[E][0] == K[F]) {
            N = 1;
            G = G[E];
            G.checked = 1;
            break}
         }
      if(!N) {
         L = 1;
         break}
      M = ce("a");
      B = ce("span");
      if(G[2]) {
         M.href = G[2]}
      else {
         M.href = "javascript:;";
         ns(M);
         M.style.textDecoration = "none";
         M.style.color = "white";
         M.style.cursor = "default"}
      if(F < K.length - 1 && G[3]) {
         B.className = "menuarrow"}
      ae(M, ct(G[4] == null ? G[1] : G[4]));
      if(F == 0) {
         M.menu = mn_path}
      else {
         M.menu = H[3]}
      M.onmouseover = Menu.show;
      M.onmouseout = Menu.hide;
      ae(B, M);
      ae(A, B);
      C = B;
      H = G;
      G = G[3];
      if(!G) {
         L = 1;
         break}
      }
   if(L && C) {
      C.className = ""}
   var J = ce("div");
   J.className = "clear";
   ae(A, J);
   ae(I, A);
   g_initPath.lastIt = H}
function g_formatTimeElapsed(D) {
   function G(L, K, J) {
      if(J && LANG.timeunitsab[K] == "") {
         J = 0}
      if(J) {
         return L + " " + LANG.timeunitsab[K]}
      else {
         return L + " " + (L == 1 ? LANG.timeunitssg[K] : LANG.timeunitspl[K])}
      }
   var E = [31557600, 2629800, 604800, 86400, 3600, 60, 1];
   var H = [1, 3, 3, - 1, 5, - 1, - 1];
   D = Math.max(D, 1);
   for(var C = 3, F = E.length; C < F; ++C) {
      if(D >= E[C]) {
         var B = C;
         var I = Math.floor(D / E[B]);
         if(H[B] !=- 1) {
            var A = H[B];
            D %= E[B];
            v2 = Math.floor(D / E[A]);
            if(v2 > 0) {
               return G(I, B, 1) + " " + G(v2, A, 1)}
            }
         return G(I, B, 0)}
      }
   return"(n/a)"}
function g_formatDateSimple(E, A) {
   function C(J) {
      return(J < 10 ? "0" + J : J)}
   var H = "", G = E.getDate(), D = E.getMonth() + 1, F = E.getFullYear();
   H += sprintf(LANG.date_simple, C(G), C(D), F);
   if(A == 1) {
      var I = E.getHours() + 1, B = E.getMinutes() + 1;
      H += LANG.date_at + C(I) + ":" + C(B)}
   return H}
function g_createGlow(A, G) {
   var D = ce("span");
   for(var C =- 1; C <= 1; ++C) {
      for(var B =- 1; B <= 1; ++B) {
         var F = ce("div");
         F.style.position = "absolute";
         F.style.whiteSpace = "nowrap";
         F.style.left = C + "px";
         F.style.top = B + "px";
         if(C == 0 && B == 0) {
            F.style.zIndex = 4}
         else {
            F.style.color = "black";
            F.style.zIndex = 2}
         ae(F, ct(A));
         ae(D, F)}
      }
   D.style.position = "relative";
   D.className = "glow" + (G != null ? " " + G : "");
   var E = ce("span");
   E.style.visibility = "hidden";
   ae(E, ct(A));
   ae(D, E);
   return D}
function g_createReputationBar(B, C) {
   var G = g_createReputationBar.P;
   if(!B) {
      B = 0}
   B += 42000;
   if(B < 0) {
      B = 0}
   else {
      if(B > 84999) {
         B = 84999}
      }
   var J = B, F, E = 0;
   for(var D = 0, I = G.length; D < I; ++D) {
      if(G[D] > J) {
         break}
      if(D < I - 1) {
         J -= G[D];
         E = D + 1}
      }
   F = G[E];
   if(!C) {
      C = J + " / " + F}
   var K = ce("a");
   K.href = "javascript:;";
   K.className = "reputation";
   var A = ce("div");
   A.className = "reputation-text";
   var L = ce("del");
   ae(L, ct(g_reputation_standings[E]));
   ae(A, L);
   var H = ce("ins");
   ae(H, ct(C));
   ae(A, H);
   ae(K, A);
   A = ce("div");
   A.className = "reputation-bar" + E;
   A.style.width = parseInt(J / F * 100) + "%";
   ae(A, ct(String.fromCharCode(160)));
   ae(K, A);
   return K}
g_createReputationBar.P = [36000, 3000, 3000, 3000, 6000, 12000, 21000, 999];
function g_createCaptcha(B) {
   var A = ce("a");
   A.href = "javascript:;";
   A.className = "captcha";
   A.title = LANG.tooltip_captcha;
   if(B & 1) {
      A.style.marginLeft = A.style.marginRight = "auto"}
   if(Browser.ie6) {
      var C = ce("img");
      C.src = "/?captcha&foo=" + Math.random();
      ae(A, C);
      A.onclick = function() {
         de(this.firstChild);
         var D = ce("img");
         D.src = "/?captcha&foo=" + Math.random();
         ae(A, D);
         this.blur()}
      }
   else {
      A.style.backgroundImage = "url(/?captcha&foo=" + Math.random() + ")";
      A.onclick = function() {
         this.style.backgroundImage = "url(/?captcha&foo=" + Math.random() + ")"}
      }
   return A}
function g_revealCaptcha(A) {
   if((g_user.permissions & 1) == 0) {
      var C = ge("klrbetkjerbt46");
      if(!C.firstChild) {
         var B = g_createCaptcha(A);
         ae(C, B);
         C.parentNode.style.display = ""}
      }
   }
function g_convertRatingToPercent(F, B, E) {
   var D = {
      12 : 1.5, 13 : 12, 14 : 15, 15 : 5, 16 : 10, 17 : 10, 18 : 8, 19 : 14, 20 : 14, 21 : 14, 22 : 10, 23 : 10, 24 : 0, 25 : 0, 26 : 0, 27 : 0, 28 : 10, 29 : 10, 30 : 10, 31 : 10, 32 : 14, 33 : 0, 34 : 0, 35 : 25, 36 : 10, 37 : 2.5, 44 : 4.69512176513672};
   if(F < 0) {
      F = 1}
   else {
      if(F > 80) {
         F = 80}
      }
   if((B == 14 || B == 12 || B == 15) && F < 34) {
      F = 34}
   if(E < 0) {
      E = 0}
   var C;
   if(D[B] == null) {
      C = 0}
   else {
      var A;
      if(F > 70) {
         A = (82 / 52) * Math.pow((131 / 63), ((F - 70) / 10))}
      else {
         if(F > 60) {
            A = (82 / (262 - 3 * F))}
         else {
            if(F > 10) {
               A = ((F - 8) / 52)}
            else {
               A = 2 / 52}
            }
         }
      C = E / D[B]/A}return C}function g_setRatingLevel(F,E,A,C){var D=prompt(sprintf(LANG.prompt_ratinglevel,1,80),E);
      if(D != null) {
         D |= 0;
         if(D != E && D >= 1 && D <= 80) {
            E = D;
            var B = g_convertRatingToPercent(E, A, C);
            B = (Math.round(B * 100) / 100);
            if(A != 12 && A != 37) {
               B += "%"}
            F.innerHTML = sprintf(LANG.tooltip_combatrating, B, E);
            F.onclick = g_setRatingLevel.bind(0, F, E, A, C)}
         }
      }
   function g_getMoneyHtml(C) {
      var B = 0, A = "";
      if(C >= 10000) {
         B = 1;
         A += '<span class="moneygold">' + Math.floor(C / 10000) + "</span>";
         C %= 10000}
      if(C >= 100) {
         if(B) {
            A += " "}
         else {
            B = 1}
         A += '<span class="moneysilver">' + Math.floor(C / 100) + "</span>";
         C %= 100}
      if(C >= 1) {
         if(B) {
            A += " "}
         else {
            B = 1}
         A += '<span class="moneycopper">' + C + "</span>"}
      return A}
   function g_getPatchVersion(E) {
      var D = g_getPatchVersion;
      var B = 0, C = D.T.length - 2, A;
      while(C > B) {
         A = Math.floor((C + B) / 2);
         if(E >= D.T[A] && E < D.T[A + 1]) {
            return D.V[A]}
         if(E >= D.T[A]) {
            B = A + 1}
         else {
            C = A - 1}
         }
      A = Math.ceil((C + B) / 2);
      return D.V[A]}
   g_getPatchVersion.V = ["1.12.0", "1.12.1", "1.12.2", "2.0.1", "2.0.3", "2.0.4", "2.0.5", "2.0.6", "2.0.7", "2.0.8", "2.0.10", "2.0.12", "2.1.0", "2.1.1", "2.1.2", "2.1.3", "2.2.0", "2.2.2", "2.2.3", "2.3.0", "2.3.2", "2.3.3", "2.4.0", "2.4.1", "2.4.2", "2.4.3", "3.0.2", "3.0.3", "3.0.8", "???"];
   g_getPatchVersion.T = [1153540800000, 1159243200000, 1160712000000, 1165294800000, 1168318800000, 1168578000000, 1168750800000, 1169528400000, 1171342800000, 1171602000000, 1173157200000, 1175572800000, 1179806400000, 1181016000000, 1182225600000, 1184040000000, 1190692800000, 1191297600000, 1191902400000, 1194930000000, 1199768400000, 1200978000000, 1206417600000, 1207022400000, 1210651200000, 1216094400000, 1223956800000, 1225774800000, 1232427600000, 9999999999999];
   function g_expandSite() {
      ge("wrapper").className = "noads";
      var D = ["topbar-expand", "header-ad", "sidebar", "infobox-ad", "contribute-ad", "pl-rightbar-ad", "blog-sidebar-medrect"];
      for(var C = 0, A = D.length; C < A; ++C) {
         var B = ge(D[C]);
         if(B) {
            de(B)}
         }
      }
   function g_insertTag(E, H, A, J) {
      var C = $(E);
      C.focus();
      if(C.selectionStart != null) {
         var K = C.selectionStart, G = C.selectionEnd, I = C.scrollLeft, D = C.scrollTop;
         var B = C.value.substring(K, G);
         if(typeof J == "function") {
            B = J(B)}
         C.value = C.value.substr(0, K) + H + B + A + C.value.substr(G);
         C.selectionStart = C.selectionEnd = G + H.length;
         C.scrollLeft = I;
         C.scrollTop = D}
      else {
         if(document.selection && document.selection.createRange) {
            var F = document.selection.createRange();
            if(F.parentElement() != C) {
               return }
            var B = F.text;
            if(typeof J == "function") {
               B = J(B)}
            F.text = H + B + A}
         }
      if(C.onkeyup) {
         C.onkeyup()}
      }
   function g_getLocaleFromDomain(B) {
      var A = g_getLocaleFromDomain.L;
      return(A[B] ? A[B] : 0)}
   g_getLocaleFromDomain.L = {
      fr : 2, de : 3, es : 6, ru : 7, wotlk : 0, ptr : 25};
   function g_getIdFromTypeName(A) {
      var B = g_getIdFromTypeName.L;
      return(B[A] ? B[A] :- 1)}
   g_getIdFromTypeName.L = {
      npc : 1, object : 2, item : 3, itemset : 4, quest : 5, spell : 6, zone : 7, faction : 8, pet : 9, achievement : 10};
   function g_getIngameLink(A, C, B) {
      prompt(LANG.prompt_ingamelink, '/script DEFAULT_CHAT_FRAME:AddMessage("' + sprintf(LANG.message_ingamelink, "\\124c" + A + "\\124H" + C + "\\124h[" + B + ']\\124h\\124r");'))}
   function g_isEmailValid(A) {
      return A.match(/^([a-z0-9._-]+)(\+[a-z0-9._-]+)?(@[a-z0-9.-]+\.[a-z]{2,4})$/i)!=null}function g_userDescription(){var C=ge("description");
      var D = (typeof g_pageInfo == "object" && g_user.name == g_pageInfo.username);
      var B = (C.childNodes.length == 0);
      if(B) {
         if(D) {
            ae(C, ct(LANG.user_nodescription2))}
         else {
            ae(C, ct(LANG.user_nodescription))}
         }
      if(D) {
         var A = ce("button"), E = ce("div");
         E.className = "pad";
         A.onclick = function() {
            location.href = "/?account#public-description"};
         if(B) {
            ae(A, ct(LANG.user_composeone))}
         else {
            ae(A, ct(LANG.user_editdescription))}
         ae(C, E);
         ae(C, A)}
      }
   function g_onAfterTyping(B, E, D) {
      var A;
      var C = function() {
         if(A) {
            clearTimeout(A);
            A = null}
         A = setTimeout(E, D)};
      B.onkeyup = C}
   function g_createOrRegex(C) {
      var E = C.split(" "), D = "";
      for(var B = 0, A = E.length; B < A; ++B) {
         if(B > 0) {
            D += "|"}
         D += E[B]}
      return new RegExp("(" + D + ")", "gi")}
   function g_addPages(J, C) {
      function L(Q, O) {
         var P;
         if(Q == C.page) {
            P = ce("span");
            P.className = "selected"}
         else {
            P = ce("a");
            P.href = (Q > 1 ? C.url + C.sep + Q + C.pound : C.url + C.pound)}
         ae(P, ct(O != null ? O : Q));
         return P}
      if(!C.pound) {
         C.pound = ""}
      if(!C.sep) {
         C.sep = "."}
      if(C.allOrNothing && C.nPages <= 1) {
         return }
      var H = (C.align && C.align == "left");
      var D = ce("div"), K, N = ce("var");
      D.className = "pages";
      if(H) {
         D.className += " pages-left"}
      if(C.nPages > 1) {
         K = ce("div");
         K.className = "pages-numbers";
         var M = Math.max(2, C.page - 3);
         var G = Math.min(C.nPages - 1, C.page + 3);
         var A = [];
         if(C.page != C.nPages) {
            A.push(L(C.page + 1, LANG.lvpage_next + String.fromCharCode(8250)))}
         A.push(L(C.nPages));
         if(G < C.nPages - 1) {
            var B = ce("span");
            ae(B, ct("..."));
            A.push(B)}
         for(var F = G; F >= M; --F) {
            A.push(L(F))}
         if(M > 2) {
            var B = ce("span");
            ae(B, ct("..."));
            A.push(B)}
         A.push(L(1));
         if(C.page != 1) {
            A.push(L(C.page - 1, String.fromCharCode(8249) + LANG.lvpage_previous))}
         if(H) {
            A.reverse()}
         for(var F = 0, I = A.length; F < I; ++F) {
            ae(K, A[F])}
         K.firstChild.style.marginRight = "0";
         K.lastChild.style.marginLeft = "0"}
      var N = ce("var");
      ae(N, ct(sprintf(LANG[C.wording[C.nItems == 1 ? 0 : 1]], C.nItems)));
      if(C.nPages > 1) {
         var B = ce("span");
         ae(B, ct(String.fromCharCode(8211)));
         ae(N, B);
         var E = ce("a");
         E.className = "gotopage";
         E.href = "javascript:;";
         ns(E);
         if(Browser.ie) {
            ae(E, ct(" "))}
         E.onclick = function() {
            var O = prompt(sprintf(LANG.prompt_gotopage, 1, C.nPages), C.page);
            if(O != null) {
               O |= 0;
               if(O != C.page && O >= 1 && O <= C.nPages) {
                  document.location.href = (O > 1 ? C.url + C.sep + O + C.pound : C.url + C.pound)}
               }
            };
         E.onmouseover = function(O) {
            Tooltip.showAtCursor(O, LANG.tooltip_gotopage, 0, 0, "q")};
         E.onmousemove = Tooltip.cursorUpdate;
         E.onmouseout = Tooltip.hide;
         ae(N, E)}
      if(H) {
         ae(D, N);
         if(K) {
            ae(D, K)}
         }
      else {
         if(K) {
            ae(D, K)}
         ae(D, N)}
      ae(J, D)}
   function co_addYourComment() {
      tabsContribute.focus(0);
      var A = gE(document.forms.addcomment, "textarea")[0];
      A.focus()}
   function co_cancelReply() {
      ge("gjkdlfgkjh436").style.display = "none";
      document.forms.addcomment.elements.replyto.value = ""}
   function co_validateForm(B) {
      var A = gE(B, "textarea")[0];
      if(Listview.funcBox.coValidate(A)) {
         if(g_user.permissions & 1) {
            return true}
         if(B.elements.captcha.value.length == 5) {
            return true}
         else {
            alert(LANG.message_codenotentered);
            B.elements.captcha.focus()}
         }
      return false}
   function ss_submitAScreenshot() {
      tabsContribute.focus(1)}
   function ss_validateForm(A) {
      if(!A.elements.screenshotfile.value.length) {
         alert(LANG.message_noscreenshot);
         return false}
      return true}
   function ss_appendSticky() {
      var J = ge("infobox-sticky");
      var F = g_pageInfo.type;
      var A = g_pageInfo.typeId;
      var H = in_array(lv_screenshots, 1, function(L) {
         return L.sticky}
      );
      if(H !=- 1) {
         var E = lv_screenshots[H];
         var I = ce("a");
         I.href = "#screenshots:id=" + E.id;
         I.onclick = function(L) {
            ScreenshotViewer.show( {
               screenshots : lv_screenshots, pos : H}
            );
            return rf2(L)};
         var C = ce("img"), B = Math.min(150 / E.width, 150 / E.height);
         C.src = "http://static.wowhead.com/uploads/screenshots/thumb/" + E.id + ".jpg";
         C.className = "border";
         ae(I, C);
         ae(J, I);
         var D = ce("div");
         var K = ce("small");
         I = ce("a");
         if(g_user.id > 0) {
            I.href = "javascript:;";
            I.onclick = ss_submitAScreenshot}
         else {
            I.href = "/?account=signin"}
         ae(I, ct(LANG.infobox_submitone));
         ae(K, I);
         ae(K, ct(" " + String.fromCharCode(160)));
         var G = ce("b");
         ae(G, ct("|"));
         ae(K, G);
         ae(K, ct(String.fromCharCode(160) + " "));
         I = ce("a");
         I.href = "javascript:;";
         I.onclick = function() {
            tabsRelated.focus( - 1);
            return false};
         ae(I, ct(sprintf(LANG.infobox_showall, lv_screenshots.length)));
         ae(K, I);
         ae(D, K);
         ae(J, D)}
      else {
         var I;
         if(g_user.id > 0) {
            I = '<a href="javascript:;" onclick="ss_submitAScreenshot(); return false">'}
         else {
            I = '<a href="/?account=signin">'}
         J.innerHTML = sprintf(LANG.infobox_noneyet, I + LANG.infobox_submitone + "</a>")}
      }
   function su_addToSaved(A) {
      /*
      if(!A) {
         return }
      var B = gc("compare_groups");
      if(!B || confirm(LANG.confirm_addtosaved)) {
         if(B) {
            A = B + ";" + A}
         sc("compare_groups", 20, A, "/", ".wowhead.com");
         document.location.href = "/?compare"}
      else {
         document.location.href = "/?compare=" + A}
      */
      return false;
      }
   function Ajax(B, C) {
      if(!B) {
         return }
      var A;
      try {
         A = new XMLHttpRequest()}
      catch(D) {
         try {
            A = new ActiveXObject("Msxml2.XMLHTTP")}
         catch(D) {
            try {
               A = new ActiveXObject("Microsoft.XMLHTTP")}
            catch(D) {
               if(window.createRequest) {
                  A = window.createRequest()}
               else {
                  alert(LANG.message_ajaxnotsupported);
                  return }
               }
            }
         }
      this.request = A;
      cO(this, C);
      this.method = this.method || (this.params && "POST") || "GET";
      A.open(this.method, B, this.async == null ? true : this.async);
      A.onreadystatechange = Ajax.onReadyStateChange.bind(this);
      if(this.method.toUpperCase() == "POST") {
         A.setRequestHeader("Content-Type", (this.contentType || "application/x-www-form-urlencoded") + "; charset=" + (this.encoding || "UTF-8"))}
      A.send(this.params)}
   Ajax.onReadyStateChange = function() {
      if(this.request.readyState == 4) {
         if(this.request.status == 0 || (this.request.status >= 200 && this.request.status < 300)) {
            this.onSuccess != null && this.onSuccess(this.request, this)}
         else {
            this.onFailure != null && this.onFailure(this.request, this)}
         if(this.onComplete != null) {
            this.onComplete(this.request, this)}
         }
      };
   function g_ajaxIshRequest(A) {
      var B = document.getElementsByTagName("head")[0];
      ae(B, ce("script", {
         type : "text/javascript", src : A}
      ))}
   var Menu = {
      iframes : [], divs : [], selection : [], show : function() {
         try {
            clearTimeout(Menu.timer);
            if(Menu.currentLink) {
               Menu._show(this)}
            else {
               if(this.className.indexOf("open") ==- 1) {
                  this.className = " open"}
               Menu.timer = setTimeout(Menu._show.bind(0, this), 100)}
            }
         catch(A) {
            }
         }
      , _show : function(B) {
         if(Menu.currentLink != B) {
            var A = ac(B);
            Menu._hide();
            Menu.selection = [ - 1];
            Menu.currentLink = B;
            Menu.showDepth(0, B.menu, A[0], A[1] + B.offsetHeight + 1, B.offsetHeight + 8, B.offsetWidth, A[1]);
            if(B.className.indexOf("open") ==- 1) {
               B.className += " open"}
            }
         else {
            Menu.truncate(0);
            Menu.clean(0)}
         }
      , showAtCursor : function(A) {
         A = $E(A);
         clearTimeout(Menu.timer);
         Menu._hide();
         Menu.selection = [ - 1];
         Menu.currentLink = null;
         var B = g_getCursorPos(A);
         Menu.showDepth(0, this.menu, B.x, B.y, 0, 0, 0)}
      , hide : function() {
         try {
            clearTimeout(Menu.timer);
            if(Menu.currentLink) {
               Menu.timer = setTimeout(Menu._hide, 333)}
            else {
               this.className = this.className.replace("open", "")}
            }
         catch(A) {
            }
         }
      , _hide : function() {
         for(var B = 0, A = Menu.selection.length; B < A; ++B) {
            Menu.divs[B].style.display = "none";
            Menu.divs[B].style.visibility = "hidden";
            if(Browser.ie6) {
               Menu.iframes[B].style.display = "none"}
            }
         Menu.selection = [];
         if(Menu.currentLink) {
            Menu.currentLink.className = Menu.currentLink.className.replace("open", "")}
         Menu.currentLink = null}
      , sepOver : function() {
         var B = this.d;
         var A = B.i;
         Menu.truncate(A);
         Menu.clean(A);
         Menu.selection[A] =- 1}
      , elemOver : function() {
         var E = this.d;
         var D = E.i;
         var C = this.i;
         var A = this.k;
         var B = this.firstChild.className == "menusub";
         Menu.truncate(D + B);
         if(B && A != Menu.selection[D]) {
            var F = ac(this);
            Menu.selection[D + 1] =- 1;
            Menu.showDepth(D + 1, E.menuArray[C][3], F[0], F[1] - 2, this.offsetHeight, this.offsetWidth - 3, 0)}
         Menu.clean(D);
         Menu.selection[D] = A;
         if(this.className.length) {
            this.className += " open"}
         else {
            this.className = "open"}
         }
      , elemClick : function(A) {
         Menu._hide();
         A()}
      , getIframe : function(A) {
         var B;
         if(Menu.iframes[A] == null) {
            B = ce("iframe");
            B.src = "javascript:0;";
            B.frameBorder = 0;
            ae(ge("layers"), B);
            Menu.iframes[A] = B}
         else {
            B = Menu.iframes[A]}
         return B}
      , getDiv : function(B, A) {
         var C;
         if(Menu.divs[B] == null) {
            C = ce("div");
            C.className = "menu";
            ae(ge("layers"), C);
            Menu.divs[B] = C}
         else {
            C = Menu.divs[B]}
         C.i = B;
         C.menuArray = A;
         return C}
      , showDepth : function(Z, V, M, L, f, N, D) {
         var q, l = Menu.getDiv(Z, V);
         while(l.firstChild) {
            de(l.firstChild)}
         var g = ce("table"), A = ce("tbody"), E = ce("tr"), I = ce("td"), Q = ce("div"), G = ce("div");
         var S = 999;
         var P = g_getWindowSize(), C = g_getScroll(), J = P.w, T = P.h, B = C.x, n = C.y;
         if(f > 0 && (Z > 0 || V.length > 20)) {
            if((25 + 1) * V.length > T - 25 - D) {
               for(var X = 2; X < 4; ++X) {
                  if(f / X * V.length + 30 < T - D) {
                     break}
                  }
               S = Math.floor(V.length / X)}
            }
         var H = 0;
         var U = 0;
         for(var X = 0, e = V.length; X < e; ++X) {
            if(V[X][0] == null) {
               var Y = ce("span");
               Y.className = "separator";
               ns(Y);
               ae(Y, ct(V[X][1]));
               Y.d = l;
               Y.onmouseover = Menu.sepOver;
               ae(G, Y)}
            else {
               var o = ce("a");
               o.d = l;
               o.k = U++;
               o.i = X;
               if(V[X][2]) {
                  if(Menu.currentLink && Menu.currentLink.menuappend) {
                     if(V[X][2].indexOf(Menu.currentLink.menuappend) ==- 1) {
                        o.href = V[X][2] + Menu.currentLink.menuappend}
                     else {
                        o.href = V[X][2]}
                     }
                  else {
                     if(typeof V[X][2] == "function") {
                        o.href = "javascript:;";
                        o.onclick = Menu.elemClick.bind(0, V[X][2]);
                        ns(o)}
                     else {
                        o.href = V[X][2]}
                     }
                  }
               else {
                  o.href = "javascript:;";
                  o.style.cursor = "default";
                  ns(o)}
               o.onmouseover = Menu.elemOver;
               var O = ce("span"), F = ce("span");
               if(V[X][3] != null) {
                  O.className = "menusub"}
               if(V[X].checked) {
                  F.className = "menucheck"}
               if(V[X].newWindow) {
                  o.target = "_blank"}
               ae(F, ct(V[X][1]));
               ae(O, F);
               ae(o, O);
               ae(G, o)}
            if(H++== S) {
               Q.onmouseover = Menu.divOver;
               Q.onmouseout = Menu.divOut;
               ae(Q, G);
               if(!Browser.ie6) {
                  var R = ce("p");
                  ae(R, ce("em"));
                  ae(R, ce("var"));
                  ae(R, ce("strong"));
                  ae(R, Q);
                  ae(I, R)}
               else {
                  ae(I, Q)}
               ae(E, I);
               I = ce("td");
               R = ce("p");
               Q = ce("div");
               G = ce("div");
               H = 0}
            }
         Q.onmouseover = Menu.divOver;
         Q.onmouseout = Menu.divOut;
         ae(Q, G);
         if(!Browser.ie6) {
            if(S != 999) {
               var R = ce("p");
               ae(R, ce("em"));
               ae(R, ce("var"));
               ae(R, ce("strong"));
               ae(R, Q);
               ae(I, R)}
            else {
               ae(l, ce("em"));
               ae(l, ce("var"));
               ae(l, ce("strong"));
               ae(I, Q)}
            }
         else {
            ae(I, Q)}
         ae(E, I);
         ae(A, E);
         ae(g, A);
         ae(l, g);
         l.style.left = l.style.top = "-2323px";
         l.style.display = "";
         var K = g.offsetWidth, W = g.offsetHeight;
         if(!Browser.ie6) {
            K += 5;
            W += 6}
         if(V.rightAligned) {
            M = M + N - K;
            if(Browser.ie) {
               M -= 3}
            }
         else {
            if(M + N + K > J) {
               M = Math.max(0, M - K)}
            else {
               if(Z > 0) {
                  M += N}
               }
            }
         if((Z > 0 || L + W > document.body.offsetHeight) && L + W > T + n) {
            L = Math.max(n + 5, T + n - W)}
         l.style.left = M + "px";
         l.style.top = L + "px";
         if(Browser.ie6) {
            q = Menu.getIframe(Z);
            q.style.left = M + "px";
            q.style.top = L + "px";
            q.style.width = K + "px";
            q.style.height = W + "px";
            q.style.display = "";
            q.style.visibility = "visible"}
         l.style.visibility = "visible";
         if(Browser.opera) {
            l.style.display = "none";
            l.style.display = ""}
         }
      , divOver : function() {
         clearTimeout(Menu.timer)}
      , divOut : function() {
         clearTimeout(Menu.timer);
         Menu.timer = setTimeout(Menu._hide, 333)}
      , truncate : function(A) {
         var B;
         while(Menu.selection.length - 1 > A) {
            B = Menu.selection.length - 1;
            Menu.divs[B].style.display = "none";
            Menu.divs[B].style.visibility = "hidden";
            if(Browser.ie6) {
               Menu.iframes[B].style.display = "none"}
            Menu.selection.pop()}
         }
      , clean : function(A) {
         for(var B = A; B < Menu.selection.length; ++B) {
            if(Menu.selection[B] !=- 1) {
               d = gE(Menu.divs[B], "a")[Menu.selection[B]];
               if(d.className.indexOf("sub") !=- 1) {
                  d.className = "sub"}
               else {
                  d.className = ""}
               Menu.selection[B] =- 1}
            }
         }
      , append : function(A, B) {
         A[2] += B;
         if(A[3] != null) {
            Menu._append(A[3], B)}
         }
      , _append : function(A, C) {
         var D, F = 0;
         for(var B = 0; B < A.length; ++B) {
            var E = A[B][2].indexOf("&filter=");
            if(E !=- 1 && C.indexOf("&filter=") == 0) {
               C = Menu._fixCollision(A[B][2].substr(E), C)}
            A[B][2] += C;
            if(A[B][3]) {
               Menu._append(A[B][3], C)}
            }
         }
      , _splitFilter : function(B) {
         var G = B.substr(8).split(";"), E = {
            };
         for(var D = 0, A = G.length; D < A; ++D) {
            var H = G[D].indexOf("="), C, F;
            if(H !=- 1) {
               C = G[D].substr(0, H);
               F = G[D].substr(H + 1)}
            else {
               C = G[D];
               F = ""}
            E[C] = F}
         return E}
      , _fixCollision : function(C, B) {
         var A = Menu._splitFilter(C), E = Menu._splitFilter(B);
         B = "";
         for(var D in E) {
            if(!A[D] && D != "sl" && D != "cl") {
               B += ";";
               B += D + "=" + E[D]}
            }
         return B}
      , fixUrls : function(G, B, E, D, F) {
         if(!F) {
            F = 0}
         for(var C = 0, A = G.length; C < A; ++C) {
            if(G[C][2] == null) {
               G[C][2] = B + G[C][0] + (E ? E : "")}
            if(G[C][3]) {
               if(D == true || (typeof D == "object" && D[F] == true)) {
                  Menu.fixUrls(G[C][3], B, E, D, F + 1)}
               else {
                  Menu.fixUrls(G[C][3], B + G[C][0] + ".", E, D, F + 1)}
               }
            }
         }
      , addButtons : function(F, E) {
         for(var C = 0, A = E.length; C < A; ++C) {
            if(E[C][0] == null) {
               continue}
            var B = ce("a"), D = ce("span");
            if(E[C][2]) {
               B.href = E[C][2]}
            else {
               B.href = "javascript:;";
               B.style.cursor = "default";
               B.style.textDecoration = "none";
               ns(B)}
            if(E[C][3] != null) {
               D.className = "menuarrowd";
               B.menu = E[C][3];
               B.onmouseover = Menu.show;
               B.onmouseout = Menu.hide}
            else {
               B.onmouseover = Menu._hide}
            ae(D, ct(E[C][1]));
            ae(B, D);
            ae(F, B)}
         }
      , explode : function(F) {
         var B = [], E = null, D;
         for(var C = 0, A = F.length; C < A; ++C) {
            if(F[C][0] != null) {
               if(E != null) {
                  D.push(F[C])}
               else {
                  B.push(F[C])}
               }
            if(E != null && (F[C][0] == null || C == A - 1)) {
               B.push([0, E[1], , D])}
            if(F[C][0] == null) {
               E = F[C];
               D = []}
            }
         return B}
      };
   function Tabs(A) {
      cO(this, A);
      if(this.parent) {
         this.parent = $(this.parent)}
      else {
         return }
      this.oldMode = (Browser.geckoVersion > 20000000 && Browser.geckoVersion <= 20060414);
      this.selectedTab =- 1;
      this.uls = [];
      this.tabs = [];
      this.nShows = 0;
      if(this.poundable == null) {
         this.poundable = 1}
      this.poundedTab = null;
      if(this.onLoad == null) {
         this.onLoad = Tabs.onLoad.bind(this)}
      if(this.onShow == null) {
         this.onShow = Tabs.onShow.bind(this)}
      if(this.onHide) {
         this.onHide = this.onHide.bind(this)}
      }
   Tabs.prototype = {
      add : function(A, D) {
         var C, B = this.tabs.length;
         C = {
            caption : A, index : B, owner : this};
         cO(C, D);
         this.tabs.push(C);
         return B}
      , focus : function(A) {
         if(A < 0) {
            A = this.tabs.length + A}
         this.forceScroll = 1;
         gE(this.uls[this.oldMode ? 0 : 2], "a")[A].onclick( {
            }
         , true);
         this.forceScroll = null}
      , show : function(C, E) {
         var B;
         if(isNaN(C) || C < 0) {
            C = 0}
         else {
            if(C >= this.tabs.length) {
               C = this.tabs.length - 1}
            }
         if(E == null && C == this.selectedTab) {
            return }
         if(this.selectedTab !=- 1) {
            B = this.tabs[this.selectedTab];
            if(this.onHide &&!this.onHide(B)) {
               return }
            if(B.onHide &&!B.onHide()) {
               return }
            }
         ++this.nShows;
         var A = this.oldMode ? 0 : 3;
         for(var D = 0; D <= A; ++D) {
            B = gE(this.uls[D], "a");
            if(this.selectedTab !=- 1) {
               B[this.selectedTab].className = ""}
            if(B[C]){B[C].className = "selected"}
            }
         B = this.tabs[C];
         if(B && B.onLoad) {
            B.onLoad();
            B.onLoad = null}
         this.onShow(this.tabs[C], this.tabs[this.selectedTab]);
         if(B && B.onShow) {
            B.onShow(this.tabs[this.selectedTab])}
         this.selectedTab = C}
      , flush : function(J) {
         if(this.oldMode) {
            var H, M, C, K;
            H = ce("ul");
            H.className = "old-tabs";
            for(var G = 0; G < this.tabs.length; ++G) {
               var D = this.tabs[G];
               M = ce("li");
               C = ce("div");
               K = ce("a");
               if(this.poundable) {
                  K.href = "#" + D.id}
               else {
                  K.href = "javascript:;"}
               ns(K);
               K.onclick = Tabs.onClick.bind(D, K);
               ae(K, ct(D.caption));
               ae(M, C);
               ae(M, K);
               ae(H, M)}
            this.uls[0] = H;
            ae(this.parent, H)}
         else {
            var L, E, K, I, A;
            var B = ce("div");
            B.className = "tabs-container";
            I = ce("div");
            I.style.visibility = "hidden";
            this.uls[0] = ce("ul");
            this.uls[0].className = "tabs";
            ae(I, this.uls[0]);
            ae(B, I);
            I = ce("div");
            I.className = "tabs-levels";
            for(var G = 1; G <= 3; ++G) {
               A = ce("div");
               A.className = "tabs-level";
               this.uls[G] = ce("ul");
               this.uls[G].className = "tabs";
               this.uls[G].style.top = ( - 30 * (3 - G)) + "px";
               ae(A, this.uls[G]);
               ae(I, A)}
            ae(B, I);
            for(var G = 0; G < this.tabs.length; ++G) {
               var D = this.tabs[G];
               for(var F = 0; F <= 3; ++F) {
                  E = ce("li");
                  K = ce("a");
                  b = ce("b");
                  if(this.poundable) {
                     K.href = "#" + D.id}
                  else {
                     K.href = "javascript:;"}
                  if(F > 0) {
                     ns(K);
                     K.onclick = Tabs.onClick.bind(D, K)}
                  if(!Browser.ie6) {
                     I = ce("div");
                     ae(I, ct(D.caption));
                     ae(K, I)}
                  ae(b, ct(D.caption));
                  ae(K, b);
                  ae(E, K);
                  ae(this.uls[F], E)}
               }
            ae(this.parent, B)}
         if(this.onLoad) {
            L = this.onLoad();
            if(L != null) {
               this.poundedTab = J = L}
            }
         this.show(J)}
      , setTabName : function(C, B) {
         var A = this.oldMode ? 0 : 3;
         for(var D = 0; D <= A; ++D) {
            _ = gE(this.uls[D], "a");
            g_setTextNodes(_[C], B)}
         }
      , setTabPound : function(C, A) {
         if(!this.poundable) {
            return }
         var B = this.oldMode ? 0 : 3;
         for(var D = 0; D <= B; ++D) {
            _ = gE(this.uls[D], "a");
            _[C].href = "#" + this.tabs[C].id + ":" + A}
         }
      };
   Tabs.onClick = function(A, E, D) {
      if(D == null && this.index == this.owner.selectedTab) {
         return }
      var C = rf2(E);
      if(C == null) {
         return }
      this.owner.show(this.index, D);
      if(this.owner.poundable) {
         var B = A.href.indexOf("#");
         B !=- 1 && location.replace(A.href.substr(B))}
      return C};
   Tabs.onLoad = function() {
      if(!this.poundable ||!location.hash.length) {
         return }
      var A = location.hash.substr(1).split(":")[0];
      if(A) {
         return in_array(this.tabs, A, function(B) {
            return B.id}
         )}
      };
   Tabs.onShow = function(E, D) {
      var B;
      if(D) {
         ge("tab-" + D.id).style.display = "none"}
      if(E){
        B = ge("tab-" + E.id);
        B.style.display = "";
      }
      if((this.nShows == 1 && this.poundedTab != null && this.poundedTab >= 0) || this.forceScroll) {
         var C, A;
         if(this.__st) {
            C = this.__st;
            A = 15}
         else {
            C = B;
            A = this.parent.offsetHeight + 15}
         if(Browser.ie) {
            setTimeout(g_scrollTo.bind(this, C, A), 1)}
         else {
            g_scrollTo(C, A)}
         }
      };
   var Icon = {
      sizes : ["small", "medium", "large"], create : function(A, J, I, D, B, H) {
         var G;
         var E = ce("div"), C = ce("div");
         E.className = "icon" + Icon.sizes[J];
         if(A != null) {
            E.style.backgroundImage = "url(http://static.wowhead.com/images/icons/" + Icon.sizes[J] + "/" + A.toLowerCase() + ".jpg)"}
         C.className = "tile";
         if(I || D) {
            var F = ce("a");
            if(I && I.length) {
               F.tooltip = I;
               F.onmouseover = Icon.over;
               F.onmouseout = Icon.out}
            if(D) {
               F.href = D}
            else {
               F.href = "javascript:;";
               ns(F)}
            ae(C, F)}
         else {
            E.ondblclick = function() {
               prompt("", A)}
            }
         if(B != null && ((B > 1 && B < 2147483647) || B.length)) {
            G = g_createGlow(B, "q1");
            G.style.right = "0";
            G.style.bottom = "0";
            G.style.position = "absolute";
            ae(C, G)}
         if(H != null && H > 0) {
            G = g_createGlow("(" + H + ")", "q");
            G.style.left = "0";
            G.style.top = "0";
            G.style.position = "absolute";
            ae(C, G)}
         ae(E, C);
         return E}
      , over : function() {
         if(this.tooltip != null) {
            Tooltip.show(this, this.tooltip, 0, 0)}
         }
      , out : function() {
         Tooltip.hide()}
      };
   var Tooltip = {
      clip : "main-contents", create : function(G) {
         var E = ce("div"), J = ce("table"), C = ce("tbody"), D = ce("tr"), B = ce("tr"), A = ce("td"), I = ce("th"), H = ce("th"), F = ce("th");
         E.className = "tooltip";
         I.style.backgroundPosition = "top right";
         H.style.backgroundPosition = "bottom left";
         F.style.backgroundPosition = "bottom right";
         if(G) {
            A.innerHTML = G}
         ae(D, A);
         ae(D, I);
         ae(C, D);
         ae(B, H);
         ae(B, F);
         ae(C, B);
         ae(J, C);
         Tooltip.icon = ce("p");
         Tooltip.icon.style.visibility = "hidden";
         ae(Tooltip.icon, ce("div"));
         ae(E, Tooltip.icon);
         ae(E, J);
         return E}
      , fix : function(D, B, E) {
         var C = gE(D, "table")[0], G = gE(C, "td")[0], F = G.childNodes;
         if(F.length >= 2 && F[0].nodeName == "TABLE" && F[1].nodeName == "TABLE") {
            F[0].style.whiteSpace = "nowrap";
            var A;
            if(F[1].offsetWidth > 300) {
               A = Math.max(300, F[0].offsetWidth) + 20}
            else {
               A = Math.max(F[0].offsetWidth, F[1].offsetWidth) + 20}
            if(A > 20) {
               D.style.width = A + "px";
               F[0].style.width = F[1].style.width = "100%";
               if(!B && D.offsetHeight > document.body.clientHeight) {
                  C.className = "shrink"}
               }
            }
         if(E) {
            D.style.visibility = "visible"}
         }
      , fixSafe : function(C, B, A) {
         if(Browser.ie) {
            setTimeout(Tooltip.fix.bind(this, C, B, A), 1)}
         else {
            Tooltip.fix(C, B, A)}
         }
      , append : function(B, A) {
         var B = $(B);
         var C = Tooltip.create(A);
         ae(B, C);
         Tooltip.fixSafe(C, 1, 1)}
      , prepare : function() {
         if(!Tooltip.tooltip) {
            var B = Tooltip.create();
            B.style.position = "absolute";
            B.style.left = B.style.top = "-2323px";
            var A = ge("layers");
            ae(A, B);
            Tooltip.tooltip = B;
            Tooltip.tooltipTable = gE(B, "table")[0];
            Tooltip.tooltipTd = gE(B, "td")[0];
            if(Browser.ie6) {
               B = ce("iframe");
               B.src = "javascript:0;";
               B.frameBorder = 0;
               ae(A, B);
               Tooltip.iframe = B}
            }
         }
      , move : function(O, M, P, K, B, A, U, H, N, T) {
         if(!Tooltip.tooltipTable) {
            return }
         var W, G = O, R = M, J = O, I = M, F = 0, E = 0, Q = g_getWindowSize(), D = g_getScroll(), L = Q.w, S = Q.h, C = D.x, V = D.y;
         if(U == null) {
            U = Tooltip.clip}
         if(H == null) {
            H = Tooltip.tooltip;
            N = Tooltip.tooltipTable.offsetWidth;
            T = Tooltip.tooltipTable.offsetHeight}
         H.style.width = N + "px";
         if(U) {
            W = ge(U);
            if(W) {
               c = ac(W);
               F = c[0];
               E = c[1];
               if(W.offsetWidth + F <= C + L) {
                  L = W.offsetWidth + F - C}
               if(W.offsetHeight + E <= V + S) {
                  S = W.offsetHeight + E - V}
               }
            }
         if(G + P + N > L) {
            G = Math.max(G - N, F) - B}
         else {
            G += P + B}
         if(G < F) {
            G = F}
         else {
            if(G + N > C + L) {
               G = C + L - N}
            }
         if(R - T > Math.max(V, E)) {
            R -= T + A}
         else {
            R += K + A}
         if(R < E) {
            R = E}
         else {
            if(R + T > V + S) {
               R = Math.max(V, V + S - T)}
            }
         if(Tooltip.iconVisible) {
            if(J >= G - 48 && J <= G && I >= R - 4 && I <= R + 48) {
               R -= 48 - (I - R)}
            }
         H.style.left = G + "px";
         H.style.top = R + "px";
         H.style.visibility = "visible";
         if(Browser.ie6 && Tooltip.iframe) {
            W = Tooltip.iframe;
            W.style.left = G + "px";
            W.style.top = R + "px";
            W.style.width = N + "px";
            W.style.height = T + "px";
            W.style.display = "";
            W.style.visibility = "visible"}
         }
      , show : function(G, E, A, F, C) {
         if(Tooltip.disabled) {
            return }
         var B;
         Tooltip.prepare();
         if(C) {
            E = '<span class="' + C + '">' + E + "</span>"}
         B = Tooltip.tooltip;
         B.style.width = "550px";
         B.style.left = "-2323px";
         B.style.top = "-2323px";
         Tooltip.tooltipTd.innerHTML = E;
         B.style.display = "";
         var D = ac(G);
         Tooltip.fix(B, 0, 0);
         Tooltip.move(D[0], D[1], G.offsetWidth, G.offsetHeight, A, F)}
      , showAtCursor : function(B, E, A, G, D) {
         if(Tooltip.disabled) {
            return }
         if(!A || A < 10) {
            A = 10}
         if(!G || G < 10) {
            G = 10}
         B = $E(B);
         Tooltip.prepare();
         if(D) {
            E = '<span class="' + D + '">' + E + "</span>"}
         var C;
         C = Tooltip.tooltip;
         C.style.width = "550px";
         C.style.left = "-2323px";
         C.style.top = "-2323px";
         Tooltip.tooltipTd.innerHTML = E;
         C.style.display = "";
         var F = g_getCursorPos(B);
         Tooltip.fix(C, 0, 0);
         Tooltip.move(F.x, F.y, 0, 0, A, G)}
      , showAtXY : function(E, A, F, D, C) {
         if(Tooltip.disabled) {
            return }
         Tooltip.prepare();
         var B;
         B = Tooltip.tooltip;
         B.style.width = "550px";
         B.style.left = "-2323px";
         B.style.top = "-2323px";
         Tooltip.tooltipTd.innerHTML = E;
         B.style.display = "";
         Tooltip.fix(B, 0, 0);
         Tooltip.move(A, F, 0, 0, D, C, null, null, null, null)}
      , cursorUpdate : function(B, A, D) {
         if(Tooltip.disabled ||!Tooltip.tooltip) {
            return }
         B = $E(B);
         if(!A || A < 10) {
            A = 10}
         if(!D || D < 10) {
            D = 10}
         var C = g_getCursorPos(B);
         Tooltip.move(C.x, C.y, 0, 0, A, D)}
      , hide : function() {
         if(Tooltip.tooltip) {
            Tooltip.tooltip.style.display = "none";
            Tooltip.tooltip.visibility = "hidden";
            Tooltip.tooltipTable.className = "";
            if(Browser.ie6) {
               Tooltip.iframe.style.display = "none"}
            Tooltip.setIcon(null)}
         }
      , setIcon : function(A) {
         Tooltip.prepare();
         if(A) {
            Tooltip.icon.style.backgroundImage = "url(http://static.wowhead.com/images/icons/medium/" + A.toLowerCase() + ".jpg)";
            Tooltip.icon.style.visibility = "visible"}
         else {
            Tooltip.icon.style.backgroundImage = "none";
            Tooltip.icon.style.visibility = "hidden"}
         Tooltip.iconVisible = A ? 1 : 0}
      };
   var g_listviews = {
      };
   function Listview(A) {
      cO(this, A);
      if(this.id) {
         var K = (this.tabs ? "tab-" : "lv-") + this.id;
         if(this.parent) {
            var H = ce("div");
            H.id = K;
            ae($(this.parent), H);
            this.container = H}
         else {
            this.container = ge(K)}
         }
      else {
         return }
      if(this.template) {
         this.template = Listview.templates[this.template]}
      else {
         return }
      g_listviews[this.id] = this;
      if(this.poundable == null) {
         if(this.template.poundable != null) {
            this.poundable = this.template.poundable}
         else {
            this.poundable = 1}
         }
      if(this.searchable == null) {
         if(this.template.searchable != null) {
            this.searchable = this.template.searchable}
         else {
            this.searchable = 0}
         }
      if(this.filtrable == null) {
         if(this.template.filtrable != null) {
            this.filtrable = this.template.filtrable}
         else {
            this.filtrable = 0}
         }
      if(this.data.length == 1) {
         this.filtrable = null;
         this.searchable = null}
      if(this.hideBands == null) {
         this.hideBands = this.template.hideBands}
      if(this.computeDataFunc == null && this.template.computeDataFunc != null) {
         this.computeDataFunc = this.template.computeDataFunc}
      if(this.createCbControls == null && this.template.createCbControls != null) {
         this.createCbControls = this.template.createCbControls}
      if(this.template.onBeforeCreate != null) {
         if(this.onBeforeCreate == null) {
            this.onBeforeCreate = this.template.onBeforeCreate}
         else {
            this.onBeforeCreate = [this.template.onBeforeCreate, this.onBeforeCreate]}
         }
      if(this.onAfterCreate == null && this.template.onAfterCreate != null) {
         this.onAfterCreate = this.template.onAfterCreate}
      if(this.createNote == null && this.template.createNote != null) {
         this.createNote = this.template.createNote}
      this.nFilters = 0;
      this.rowOffset = 0;
      if(this.mode == null) {
         this.mode = this.template.mode}
      this.mode |= 0;
      if(this.nItemsPerPage == null) {
         var N = this.template.nItemsPerPage;
         this.nItemsPerPage = (N != null ? N : 50)}
      this.nItemsPerPage |= 0;
      if(this.nItemsPerPage <= 0) {
         this.nItemsPerPage = 0}
      this.nRowsVisible = this.data.length;
      if(this.mode == 3) {
         if(this.nItemsPerRow == null) {
            var M = this.template.nItemsPerRow;
            this.nItemsPerRow = (M != null ? M : 4)}
         this.nItemsPerRow |= 0;
         if(this.nItemsPerRow <= 1) {
            this.nItemsPerRow = 1}
         }
      else {
         this.nItemsPerRow = 1}
      this.columns = this.template.columns.slice(0);
      if(this.extraCols != null) {
         for(var D = 0, G = this.extraCols.length; D < G; ++D) {
            var I = null;
            var C = this.extraCols[D];
            if(C.after || C.before) {
               var F = in_array(this.columns, (C.after ? C.after : C.before), function(O) {
                  return O.id}
               );
               if(F !=- 1) {
                  I = (C.after ? F + 1 : F - 1)}
               }
            if(I == null) {
               I = this.columns.length}
            this.columns.splice(I, 0, C)}
         }
      this.visibility = [];
      var L = [], J = [];
      if(this.visibleCols != null) {
         array_walk(this.visibleCols, function(O) {
            L[O] = 1}
         )}
      if(this.hiddenCols != null) {
         array_walk(this.hiddenCols, function(O) {
            J[O] = 1}
         )}
      for(var D = 0, G = this.columns.length; D < G; ++D) {
         var C = this.columns[D];
         if(L[C.id] != null || (!C.hidden && J[C.id] == null)) {
            this.visibility.push(D)}
         }
      if(this.sort == null && this.template.sort) {
         this.sort = this.template.sort.slice(0)}
      else {
         if(this.sort != null) {
            var B = this.sort;
            this.sort = [];
            for(var D = 0, G = B.length; D < G; ++D) {
               var C = parseInt(B[D]);
               if(isNaN(C)) {
                  var E = 0;
                  if(B[D].charAt(0) == "-") {
                     E = 1;
                     B[D] = B[D].substring(1)}
                  var F = in_array(this.columns, B[D], function(O) {
                     return O.id}
                  );
                  if(F !=- 1) {
                     if(E) {
                        this.sort.push( - (F + 1))}
                     else {
                        this.sort.push(F + 1)}
                     }
                  }
               else {
                  this.sort.push(C)}
               }
            }
         }
      if(this.hideCount == null) {
         this.hideCount = this.template.hideCount}
      this.hideCount |= 0;
      if(this.tabs) {
         this.tabIndex = this.tabs.add(this.getTabName(), {
            id : this.id, onLoad : this.initialize.bind(this)}
         )}
      else {
         this.initialize()}
      }
   Listview.prototype = {
      initialize : function() {
         if(!this.data.length) {
            this.noData = ce("div");
            this.noData.className = "listview-nodata text";
            ae(this.container, this.noData);
            this.showNoData();
            return }
         if(this.computeDataFunc != null) {
            for(var E = 0, C = this.data.length; E < C; ++E) {
               this.computeDataFunc(this.data[E])}
            }
         if(this.tabs) {
            this.pounded = (this.tabs.poundedTab == this.tabIndex);
            if(this.pounded) {
               this.readPound()}
            }
         else {
            this.readPound()}
         this.updateSortIndex();
         var B;
         if(this.onBeforeCreate != null) {
            if(typeof this.onBeforeCreate == "function") {
               B = this.onBeforeCreate()}
            else {
               for(var E = 0; E < this.onBeforeCreate.length; ++E) {
                  (this.onBeforeCreate[E].bind(this))()}
               }
            }
         this.noData = ce("div");
         this.noData.className = "listview-nodata text";
         if(this.mode == 2) {
            this.mainDiv = ce("div");
            this.mainDiv.className = "listview-mode2";
            this.refreshRows();
            this.createBands(this.mainDiv, this.noData)}
         else {
            this.table = ce("table");
            this.thead = ce("thead");
            this.tbody = ce("tbody");
            if(this.mode == 3) {
               this.tbody.className = "listview-mode3";
               var A = ce("colgroup");
               var F = (100 / this.nItemsPerRow) + "%";
               for(var E = 0; E < this.nItemsPerRow; ++E) {
                  var D = ce("col");
                  D.style.width = F;
                  ae(A, D)}
               ae(this.table, A)}
            else {
               this.tbody.className = "listview-std";
               this.createHeader();
               this.updateSortArrow()}
            ae(this.table, this.thead);
            ae(this.table, this.tbody);
            this.refreshRows();
            this.createBands(this.table, this.noData);
            if(this.mode == 1 && Browser.ie) {
               setTimeout(Listview.cbIeFix.bind(this), 1)}
            }
         if(this.onAfterCreate != null) {
            this.onAfterCreate(B)}
         }
      , createHeader : function() {
         var H = ce("tr");
         if(this.mode == 1) {
            var C = ce("th");
            var B = ce("div");
            var J = ce("a");
            C.style.width = "33px";
            if(this.poundable == 2) {
               J.style.cursor = "default";
               J.style.backgroundColor = "#585858"}
            J.href = "javascript:;";
            J.className = "listview-cb";
            ns(J);
            ae(J, ct(String.fromCharCode(160)));
            ae(B, J);
            ae(C, B);
            ae(H, C)}
         for(var E = 0, G = this.visibility.length; E < G; ++E) {
            var I = this.visibility[E];
            var D = this.columns[I];
            var C = ce("th");
            D.__th = C;
            var B = ce("div");
            var J = ce("a");
            var A = ce("span");
            var F = ce("span");
            if(this.poundable == 2 || this.data.length == 1) {
               J.style.cursor = "default";
               J.style.backgroundColor = "#585858"}
            J.href = "javascript:;";
            if(this.poundable != 2 && this.data.length > 1) {
               if(this.filtrable && (D.filtrable == null || D.filtrable)) {
                  J.onmouseup = Listview.headerClick.bind(this, D, I);
                  J.onclick = J.oncontextmenu = rf}
               else {
                  J.onclick = this.sortBy.bind(this, I + 1)}
               J.onmouseover = Listview.headerOver.bind(this, J, D);
               J.onmouseout = Tooltip.hide}
            ns(J);
            if(D.width != null) {
               C.style.width = D.width}
            if(D.align != null) {
               C.style.textAlign = D.align}
            if(D.span != null) {
               C.colSpan = D.span}
            ae(F, ct(D.name));
            ae(A, F);
            ae(J, A);
            ae(B, J);
            ae(C, B);
            ae(H, C)}
         ae(this.thead, H)}
      , createBands : function(F, I) {
         var J = ce("div"), G = ce("div");
         J.className = "listview-band-top";
         G.className = "listview-band-bottom";
         this.navTop = this.createNav();
         this.navBot = this.createNav();
         var K = ce("div"), H = ce("div");
         K.className = H.className = "listview-note";
         if(this.note) {
            K.innerHTML = this.note}
         else {
            if(this.createNote) {
               this.createNote(K, H)}
            }
         if(!K.firstChild && this.mode != 1) {
            ae(K, ct(String.fromCharCode(160)))}
         if(this.mode != 1) {
            ae(H, ct(String.fromCharCode(160)))}
         ae(J, this.navTop);
         if(this.searchable) {
            var A = ce("span");
            A.className = "listview-quicksearch";
            var B = this.updateFilters.bind(this, 1);
            var C = (this._truncated ? "search-within-results2" : "search-within-results");
            var E = ce("a");
            E.href = "javascript:;";
            E.onclick = function() {
               var L = this.nextSibling;
               L.value = "";
               L.className = C;
               B()};
            E.style.display = "none";
            ae(E, ce("span"));
            ae(A, E);
            ns(E);
            var D = ce("input");
            D.setAttribute("type", "text");
            D.className = C;
            D.style.width = (this._truncated ? "19em" : "15em");
            g_onAfterTyping(D, B, 333);
            D.onmouseover = function() {
               if(trim(this.value) != "") {
                  this.className = ""}
               };
            D.onfocus = function() {
               this.className = ""};
            D.onblur = function() {
               if(trim(this.value) == "") {
                  this.className = C;
                  this.value = ""}
               };
            if(Browser.ie) {
               setTimeout(function() {
                  D.value = ""}
               , 1)}
            ae(A, D);
            this.quickSearchBox = D;
            this.quickSearchClear = E;
            ae(J, A)}
         ae(G, this.navBot);
         ae(J, K);
         ae(G, H);
         if(this.mode == 1) {
            if(this.note) {
               K.style.paddingBottom = "5px"}
            ae(J, this.createCbBar(1));
            ae(G, this.createCbBar(0))}
         this.bandTop = J;
         this.bandBot = G;
         this.updateNav();
         if(this.hideBands != null) {
            J.style.display = G.style.display = "none"}
         ae(this.container, this.bandTop);
         ae(this.container, F);
         ae(this.container, I);
         ae(this.container, this.bandBot)}
      , createNav : function() {
         var B = ce("div");
         B.className = "listview-nav";
         var D = ce("a"), C = ce("a"), A = ce("a"), I = ce("a");
         D.href = C.href = A.href = I.href = "javascript:;";
         ae(D, ct(String.fromCharCode(171) + LANG.lvpage_first));
         ae(C, ct(String.fromCharCode(8249) + LANG.lvpage_previous));
         ae(A, ct(LANG.lvpage_next + String.fromCharCode(8250)));
         ae(I, ct(LANG.lvpage_last + String.fromCharCode(187)));
         ns(D);
         ns(C);
         ns(A);
         ns(I);
         D.onclick = this.firstPage.bind(this);
         C.onclick = this.previousPage.bind(this);
         A.onclick = this.nextPage.bind(this);
         I.onclick = this.lastPage.bind(this);
         var H = ce("span");
         var G = ce("b"), F = ce("b"), E = ce("b");
         ae(G, ct("a"));
         ae(F, ct("a"));
         ae(E, ct("a"));
         ae(H, G);
         ae(H, ct(LANG.hyphen));
         ae(H, F);
         ae(H, ct(LANG.lvpage_of));
         ae(H, E);
         ae(B, D);
         ae(B, C);
         ae(B, H);
         ae(B, A);
         ae(B, I);
         return B}
      , createCbBar : function(E) {
         var G = ce("div");
         var C = ce("div");
         var B = ce("a"), A = ce("a"), F = ce("a");
         B.href = A.href = F.href = "javascript:;";
         ae(B, ct("All"));
         ae(A, ct("None"));
         ae(F, ct("Inverse"));
         B.onclick = Listview.cbSelect.bind(this, true);
         A.onclick = Listview.cbSelect.bind(this, false);
         F.onclick = Listview.cbSelect.bind(this, null);
         ns(B);
         ns(A);
         ns(F);
         ae(C, ct("Select: "));
         ae(C, B);
         ae(C, ct(LANG.comma));
         ae(C, A);
         ae(C, ct(LANG.comma));
         ae(C, F);
         if(this.createCbControls) {
            var D = ce("div");
            D.style.paddingBottom = "6px";
            this.createCbControls(D, C, E);
            ae(G, D)}
         ae(G, C);
         return G}
      , refreshRows : function() {
         var H = this.nRowsVisible;
         var L = (this.mode == 2 ? this.mainDiv : this.tbody);
         while(L.firstChild) {
            L.removeChild(L.firstChild)}
         if(H > 0) {
            var K, B, C;
            if(this.nItemsPerPage > 0) {
               K = this.rowOffset;
               B = Math.min(H, this.rowOffset + this.nItemsPerPage);
               if(this.filtered && this.rowOffset > 0) {
                  for(var F = 0, G = 0; F < this.data.length && G < this.rowOffset; ++F) {
                     if(this.data[F].__hidden) {
                        ++K}
                     else {
                        ++G}
                     }
                  B += (K - this.rowOffset)}
               }
            else {
               K = 0;
               B = H}
            var A = B - K;
            if(this.mode == 2) {
               for(var E = 0; E < A; ++E) {
                  var F = K + E;
                  if(!this.data[F]) {
                     break}
                  if(this.data[F].__hidden) {
                     ++A;
                     continue}
                  ae(this.mainDiv, this.getDiv(F))}
               }
            else {
               if(this.mode == 3) {
                  var D = 0, I = ce("tr");
                  for(var E = 0; E < A; ++E) {
                     var F = K + E;
                     if(this.data[F].__hidden) {
                        ++A;
                        continue}
                     ae(I, this.getCell(F));
                     if(++D == this.nItemsPerRow) {
                        ae(this.tbody, I);
                        if(E + 1 < A) {
                           I = ce("tr")}
                        D = 0}
                     }
                  if(D != 0) {
                     for(; D < 4; ++D) {
                        var J = ce("td");
                        J.className = "empty-cell";
                        ae(I, J)}
                     ae(this.tbody, I)}
                  }
               else {
                  for(var E = 0; E < A; ++E) {
                     var F = K + E;
                     if(this.data[F].__hidden) {
                        ++A;
                        continue}
                     ae(this.tbody, this.getRow(F))}
                  }
               }
            this.noData.style.display = "none";
            L.style.display = ""}
         else {
            if(!this.filtered) {
               this.bandTop.style.display = this.bandBot.style.display = "none"}
            this.noData.style.display = "";
            L.style.display = "none";
            this.showNoData()}
         }
      , showNoData : function() {
         var B = this.noData;
         while(B.firstChild) {
            de(B.firstChild)}
         var A =- 1;
         if(this.template.onNoData) {
            A = (this.template.onNoData.bind(this, B))()}
         if(A ==- 1) {
            ae(this.noData, ct(this.filtered ? LANG.lvnodata2 : LANG.lvnodata))}
         }
      , getDiv : function(B) {
         var A = this.data[B];
         if(A.__div == null) {
            this.createDiv(A, B)}
         return A.__div}
      , createDiv : function(A, B) {
         var C = ce("div");
         A.__div = C;
         (this.template.compute.bind(this, A, C, B))()}
      , getCell : function(B) {
         var A = this.data[B];
         if(A.__div == null) {
            this.createCell(A, B)}
         return A.__td}
      , createCell : function(A, B) {
         var C = ce("td");
         A.__td = C;
         (this.template.compute.bind(this, A, C, B))();
         if(this.template.getItemLink) {
            C.onclick = this.itemClick.bind(this, A)}
         if(Browser.ie6) {
            C.onmouseover = Listview.itemOver;
            C.onmouseout = Listview.itemOut}
         }
      , getRow : function(A) {
         var B = this.data[A];
         if(B.__tr == null) {
            this.createRow(B)}
         return B.__tr}
      , createRow : function(I) {
         var F = ce("tr");
         I.__tr = F;
         
         if(this.mode == 1) {
            var B = ce("td");
            if(!I.__nochk) {
            /*
               B.className = "listview-cb";
               B.onclick = Listview.cbCellClick;
               var C = ce("input");
               ns(C);
               C.type = "checkbox";
               C.onclick = Listview.cbClick;
               if(I.__chk) {
                  C.checked = true;
                  if(Browser.ie) {
                     C.defaultChecked = true}
                  }
               I.__cb = C;
               ae(B, C)
            */
               ae(B, ce("hidden"))
            }
            ae(F, B)
          }
         
         for(var D = 0, E = this.visibility.length; D < E; ++D) {
            var G = this.visibility[D];
            var A = this.columns[G];
            var B = ce("td");
            if(A.align != null) {
               B.style.textAlign = A.align}
            var H = null;
            if(A.compute) {
               H = (A.compute.bind(this, I, B, F, G))()}
            else {
               if(I[A.value] != null) {
                  ae(B, ct(I[A.value]))}
               else {
                  H =- 1}
               }
            if(H !=- 1 && H != null) {
               B.insertBefore(ct(H), B.firstChild)}
            ae(F, B)}
         if(this.mode == 1 && I.__chk) {
            F.className = "checked"}
         if(this.template.getItemLink) {
            F.onclick = this.itemClick.bind(this, I)}
         if(Browser.ie6) {
            F.onmouseover = Listview.itemOver;
            F.onmouseout = Listview.itemOut}
         }
      , itemClick : function(A, D) {
         D = $E(D);
         var B = 0, C = D._target;
         while(C && B < 3) {
            if(C.nodeName == "A") {
               return }
            C = C.parentNode}
         location.href = this.template.getItemLink(A)}
      , validatePage : function() {
         var C = this.nItemsPerPage, B = this.rowOffset, A = this.data.length;
         if(B < 0) {
            this.rowOffset = 0}
         else {
            this.rowOffset = this.getRowOffset(B + C > A ? A - 1 : B)}
         }
      , getRowOffset : function(B) {
         var A = this.nItemsPerPage;
         return(A > 0 && B > 0 ? Math.floor(B / A) * A : 0)}
      , resetRowVisibility : function() {
         for(var B = 0, A = this.data.length; B < A; ++B) {
            this.data[B].__hidden = false}
         this.filtered = 0;
         this.rowOffset = 0;
         this.nRowsVisible = this.data.length}
      , getColText : function(B, A) {
         if(A.getVisibleText) {
            return A.getVisibleText(B)}
         if(A.getValue) {
            return A.getValue(B)}
         if(A.value) {
            return B[A.value]}
         if(A.compute) {
            return A.compute(B)}
         return""}
      , updateFilters : function(A) {
         this.resetRowVisibility();
         var G = (this.quickSearchBox ? trim(this.quickSearchBox.value) : ""), P, F;
         if(G) {
            if(this.quickSearchBox) {
               this.quickSearchClear.style.display = ""}
            G = G.toLowerCase().replace(/\s+/g," ");
            P = G.split(" ");
            F = P.length}
         else {
            if(this.quickSearchBox) {
               this.quickSearchClear.style.display = "none"}
            }
         if(!G && this.nFilters == 0) {
            if(A) {
               this.updateNav();
               this.refreshRows()}
            return }
         var R = {
            1 : function(Y, Z) {
               return Y > Z}
            , 2 : function(Y, Z) {
               return Y == Z}
            , 3 : function(Y, Z) {
               return Y < Z}
            , 4 : function(Y, Z) {
               return Y >= Z}
            , 5 : function(Y, Z) {
               return Y <= Z}
            , 6 : function(Y, a, Z) {
               return a <= Y && Y <= Z}
            };
         var N = {
            1 : function(Z, Y, a) {
               return Y > a}
            , 2 : function(Z, Y, a) {
               return Z <= a && a <= Y}
            , 3 : function(Z, Y, a) {
               return Z < a}
            , 4 : function(Z, Y, a) {
               return Y >= a}
            , 5 : function(Z, Y, a) {
               return Z <= a}
            , 6 : function(Z, Y, e, a) {
               return e <= Y && Z <= a}
            };
         var O = 0;
         for(var U = 0, V = this.data.length; U < V; ++U) {
            var D = this.data[U], K = 0;
            nMatches = 0, matches = [];
            D.__hidden = true;
            for(var T = 0, E = this.visibility.length; T < E; ++T) {
               var L = this.visibility[T];
               var B = this.columns[L];
               if(B.__filter) {
                  var I = B.__filter, H = false;
                  if(B.type == null || B.type == "num") {
                     var Q = null;
                     if(B.getValue) {
                        Q = B.getValue(D)}
                     else {
                        if(B.value) {
                           Q = parseFloat(D[B.value])}
                        }
                     if(!Q) {
                        Q = 0}
                     H = (R[I.type])(Q, I.value, I.value2)}
                  else {
                     if(B.type == "range") {
                        var M = B.getMinValue(D), X = B.getMaxValue(D);
                        H = (N[I.type])(M, X, I.value, I.value2)}
                     else {
                        var J = this.getColText(D, B);
                        if(J) {
                           J = J.toString();
                           if(I.invert) {
                              H = J.match(I.regex) != null}
                           else {
                              J = J.toLowerCase();
                              var W = 0;
                              for(var S = 0, C = I.words.length; S < C; ++S) {
                                 if(J.indexOf(I.words[S]) !=- 1) {
                                    ++W}
                                 else {
                                    break}
                                 }
                              H = (W == I.words.length)}
                           }
                        }
                     }
                  if(I.invert) {
                     H =!H}
                  if(H) {
                     ++K}
                  else {
                     break}
                  }
               if(G) {
                  var J = this.getColText(D, B);
                  if(J) {
                     J = J.toString().toLowerCase();
                     for(var S = 0, C = P.length; S < C; ++S) {
                        if(!matches[S]) {
                           if(J.indexOf(P[S]) !=- 1) {
                              matches[S] = 1;
                              ++nMatches}
                           }
                        }
                     }
                  }
               }
            if((!G || nMatches == F) && (this.nFilters == 0 || K == this.nFilters)) {
               D.__hidden = false;
               ++O}
            }
         this.filtered = (O < this.data.length ? 1 : 0);
         this.nRowsVisible = O;
         if(A) {
            this.updateNav();
            this.refreshRows()}
         }
      , changePage : function() {
         this.validatePage();
         this.refreshRows();
         this.updateNav();
         this.updatePound();
         var A = g_getScroll(), B = ac(this.container);
         if(A.y > B[1]) {
            scrollTo(A.x, B[1])}
         }
      , firstPage : function() {
         this.rowOffset = 0;
         this.changePage();
         return false}
      , previousPage : function() {
         this.rowOffset -= this.nItemsPerPage;
         this.changePage();
         return false}
      , nextPage : function() {
         this.rowOffset += this.nItemsPerPage;
         this.changePage();
         return false}
      , lastPage : function() {
         this.rowOffset = 99999999;
         this.changePage();
         return false}
      , addSort : function(A, C) {
         var B = in_array(A, Math.abs(C), function(D) {
            return Math.abs(D)}
         );
         if(B !=- 1) {
            C = A[B];
            A.splice(B, 1)}
         A.splice(0, 0, C)}
      , sortBy : function(A) {
         if(this.poundable == 2 || A <= 0 || A > this.columns.length) {
            return }
         if(Math.abs(this.sort[0]) == A) {
            this.sort[0] =- this.sort[0]}
         else {
            this.addSort(this.sort, (this.columns[A - 1].type == "text" ? 1 :- 1) * A)}
         this.applySort();
         if(this.template.onSort) {
            (this.template.onSort.bind(this))()}
         this.refreshRows();
         if(this.mode != 2) {
            this.updateSortArrow()}
         this.updatePound()}
      , applySort : function() {
         Listview.sort = this.sort;
         Listview.columns = this.columns;
         if(this.indexCreated) {
            this.data.sort(Listview.sortIndexedRows)}
         else {
            this.data.sort(Listview.sortRows)}
         this.updateSortIndex()}
      , setSort : function(B, A, C) {
         if(this.sort.toString() != B.toString()) {
            this.sort = B;
            this.applySort();
            if(A) {
               this.refreshRows()}
            if(C) {
               this.updatePound()}
            }
         }
      , readPound : function() {
         if(!this.poundable ||!location.hash.length) {
            return }
         var B = location.hash.substr(1);
         if(this.tabs) {
            var G = B.indexOf(":");
            if(G ==- 1) {
               return }
            B = B.substr(G + 1)}
         var A = parseInt(B);
         if(!isNaN(A)) {
            this.rowOffset = A;
            this.validatePage();
            if(this.poundable != 2) {
               var D = [];
               var F = B.match(/(\+|\-)[0-9]+/g);
               if(F != null) {
                  for(var C = F.length - 1; C >= 0; --C) {
                     var E = parseInt(F[C]) | 0;
                     var B = Math.abs(E);
                     if(B <= 0 || B > this.columns.length) {
                        break}
                     this.addSort(D, E)}
                  this.setSort(D, false, false)}
               }
            if(this.tabs) {
               this.tabs.setTabPound(this.tabIndex, this.getTabPound())}
            }
         }
      , updateSortArrow : function() {
         if(!this.sort.length) {
            return }
         var A = in_array(this.visibility, Math.abs(this.sort[0]) - 1);
         if(A ==- 1) {
            return }
         if(this.mode == 1) {
            ++A}
         var B = this.thead.firstChild.childNodes[A].firstChild.firstChild.firstChild;
         if(this.lsa && this.lsa != B) {
            this.lsa.className = ""}
         B.className = (this.sort[0] < 0 ? "sortdesc" : "sortasc");
         this.lsa = B}
      , updateSortIndex : function() {
         var B = this.data;
         for(var C = 0, A = B.length; C < A; ++C) {
            B[C].__si = C}
         this.indexCreated = true}
      , updateTabName : function() {
         if(this.tabIndex != null) {
            this.tabs.setTabName(this.tabIndex, this.getTabName())}
         }
      , updatePound : function() {
         if(!this.poundable) {
            return }
         var A = this.getTabPound();
         if(this.tabs) {
            this.tabs.setTabPound(this.tabIndex, A);
            location.replace("#" + this.id + ":" + A)}
         else {
            location.replace("#" + A)}
         }
      , updateNav : function() {
         var D = [this.navTop, this.navBot];
         var J = this.nItemsPerPage, H = this.rowOffset, E = this.nRowsVisible;
         var B = 0, F = 0, C = 0, I = 0;
         if(E > 0) {
            D[0].style.display = D[1].style.display = ""}
         else {
            D[0].style.display = D[1].style.display = "none"}
         if(J) {
            if(H > 0) {
               F = 1;
               if(H >= J + J) {
                  B = 1}
               }
            if(H + J < E) {
               C = 1;
               if(H + J + J < E) {
                  I = 1}
               }
            }
         for(var A = 0; A < 2; ++A) {
            var G = D[A].childNodes;
            G[0].style.display = (B ? "" : "none");
            G[1].style.display = (F ? "" : "none");
            G[3].style.display = (C ? "" : "none");
            G[4].style.display = (I ? "" : "none");
            G = G[2].childNodes;
            G[0].firstChild.nodeValue = H + 1;
            G[2].firstChild.nodeValue = J ? Math.min(H + J, E) : E;
            G[4].firstChild.nodeValue = E}
         }
      , getTabName : function() {
         var A = this.name;
         var B = this.data.length;
         if(B > 0 &&!this.hideCount) {
            A += sprintf(LANG.qty, B)}
         return A}
      , getTabPound : function() {
         var A = "";
         A += this.rowOffset;
         if(this.poundable != 2 && this.sort.length) {
            A += ("+" + this.sort.join("+")).replace(/\+\-/g,"-")}return A},getCheckedRows:function(){var D=[];
            for(var C = 0, A = this.data.length; C < A; ++C) {
               var B = this.data[C];
               if((B.__cb && B.__cb.checked) || (!B.__cb && B.__chk)) {
                  D.push(B)}
               }
            return D}
         , deleteRows : function(M) {
            if(!M.length) {
               return }
            var C = 0;
            var H = 0;
            for(var E = M.length - 1; E >= 0; --E) {
               var L = M[E];
               var G =- 1;
               if(L.__tr && L.__tr.rowIndex > 0) {
                  G = this.rowOffset + L.__tr.rowIndex - 1}
               else {
                  G = in_array(this.data, L);
                  H = 1}
               if(G ==- 1) {
                  continue}
               ++C;
               var K = this.nItemsPerPage;
               var F = this.rowOffset;
               var J = F + (K > 0 ? K : this.data.length) - 1;
               if(G >= F && G <= J) {
                  if(this.mode == 2) {
                     de(L.__div)}
                  else {
                     de(L.__tr)}
                  }
               this.data.splice(G, 1);
               this.nRowsVisible--}
            if(C == 0) {
               return }
            this.updateTabName();
            if(this.rowOffset >= this.data.length) {
               this.previousPage()}
            else {
               if(this.data.length == 0 || H) {
                  this.refreshRows()}
               this.updateNav();
               var K = this.nItemsPerPage;
               var A = (this.mode == 2 ? this.mainDiv : this.tbody);
               var B = (this.mode == 2 ? this.getDiv : this.getRow);
               var I = A.childNodes.length;
               if(K > 0 && this.rowOffset + I < this.data.length) {
                  for(var E = this.rowOffset + I, D = E + K - I; E < D; ++E) {
                     ae(A, B(E))}
                  }
               }
            }
         };
      Listview.sortRows = function(C, B) {
         var G = Listview.sort, H = Listview.columns;
         for(var F = 0, A = G.length; F < A; ++F) {
            var E, D = H[Math.abs(G[F]) - 1];
            if(D.sortFunc) {
               E = D.sortFunc(C, B, G[F])}
            else {
               E = strcmp(C[D.value], B[D.value])}
            if(E != 0) {
               return E * G[F]}
            }
         return 0}
      , Listview.sortIndexedRows = function(B, A) {
         var E = Listview.sort, F = Listview.columns;
         var D, C = F[Math.abs(E[0]) - 1];
         if(C.sortFunc) {
            D = C.sortFunc(B, A, E[0])}
         else {
            D = strcmp(B[C.value], A[C.value])}
         if(D != 0) {
            return D * E[0]}
         return B.__si - A.__si}
      , Listview.cbSelect = function(B) {
         for(var D = 0, A = this.data.length; D < A; ++D) {
            var C = this.data[D];
            var F = B;
            if(!C.__nochk && C.__tr) {
               var E = C.__tr.firstChild.firstChild;
               if(F == null) {
                  F =!E.checked}
               if(E.checked != F) {
                  E.checked = F;
                  C.__tr.className = (E.checked ? "checked" : "");
                  if(Browser.ie) {
                     E.defaultChecked = F;
                     if(Browser.ie6) {
                        (Listview.itemOut.bind(C.__tr))()}
                     }
                  }
               }
            else {
               if(F == null) {
                  F = true}
               }
            C.__chk = F}
         };
      Listview.cbClick = function(A) {
         setTimeout(Listview.cbUpdate.bind(0, 0, this, this.parentNode.parentNode), 1);
         sp(A)};
      Listview.cbCellClick = function(A) {
         setTimeout(Listview.cbUpdate.bind(0, 1, this.firstChild, this.parentNode), 1);
         sp(A)};
      Listview.cbIeFix = function() {
         var D = gE(this.tbody, "tr");
         for(var C = 0, A = D.length; C < A; ++C) {
            var B = D[C].firstChild.firstChild;
            if(B) {
               B.checked = B.defaultChecked = false}
            }
         };
      Listview.cbUpdate = function(B, A, C) {
         if(B) {
            A.checked =!A.checked}
         C.className = (A.checked ? "checked" : "");
         if(Browser.ie) {
            A.defaultChecked = A.checked;
            if(Browser.ie6) {
               (Listview.itemOver.bind(C))()}
            }
         };
      Listview.itemOver = function() {
         this.style.backgroundColor = (this.className == "checked" ? "#2C2C2C" : "#202020")};
      Listview.itemOut = function() {
         this.style.backgroundColor = (this.className == "checked" ? "#242424" : "transparent")};
      Listview.headerClick = function(A, B, C) {
         C = $E(C);
         if(C._button >= 2 || C.shiftKey || C.ctrlKey) {
            Tooltip.hide();
            setTimeout(Listview.headerFilter.bind(this, A), 1)}
         else {
            this.sortBy(B + 1)}
         return false};
      Listview.headerFilter = function(B) {
         var H = "";
         if(B.__filter) {
            if(B.__filter.invert) {
               H += "!"}
            H += B.__filter.text}
         var D = prompt(sprintf(LANG.prompt_colfilter1 + (B.type == "text" ? LANG.prompt_colfilter2 : LANG.prompt_colfilter3), B.name), H);
         if(D != null) {
            var A = {
               text : "", type :- 1};
            D = trim(D.replace(/\s+/g," "));
            if(D) {
               if(D.charAt(0) == "!" || D.charAt(0) == "-") {
                  A.invert = 1;
                  D = D.substr(1)}
               if(B.type == "text") {
                  A.type = 0;
                  A.text = D;
                  if(A.invert) {
                     A.regex = g_createOrRegex(D)}
                  else {
                     A.words = D.toLowerCase().split(" ")}
                  }
               else {
                  var I, E;
                  if(D.match(/(>|=|<|>=|<=)\s*([0-9\.]+)/)){I=parseFloat(RegExp.$2);
                  if(!isNaN(I)) {
                     switch(RegExp.$1) {
                        case">" : A.type = 1;
                        break;
                        case"=" : A.type = 2;
                        break;
                        case"<" : A.type = 3;
                        break;
                        case">=" : A.type = 4;
                        break;
                        case"<=" : A.type = 5;
                        break}
                     A.value = I;
                     A.text = RegExp.$1 + " " + I}
                  }
               else {
                  if(D.match(/([0-9\.]+)\s*\-\s*([0-9\.]+)/)){I=parseFloat(RegExp.$1);
                  E = parseFloat(RegExp.$2);
                  if(!isNaN(I) &&!isNaN(E)) {
                     if(I > E) {
                        var F = I;
                        I = E;
                        E = F}
                     if(I == E) {
                        A.type = 2;
                        A.value = I;
                        A.text = "= " + I}
                     else {
                        A.type = 6;
                        A.value = I;
                        A.value2 = E;
                        A.text = I + " - " + E}
                     }
                  }
               else {
                  var C = D.toLowerCase().split(" ");
                  if(C.length == 1 &&!isNaN(I = parseFloat(C[0]))) {
                     A.type = 2;
                     A.value = I;
                     A.text = "= " + I}
                  else {
                     if(B.type == "text") {
                        A.type = 0;
                        A.text = D;
                        if(A.invert) {
                           A.regex = g_createOrRegex(D)}
                        else {
                           A.words = C}
                        }
                     }
                  }
               }
            }
         if(A.type ==- 1) {
            alert(LANG.message_invalidfilter);
            return }
         }
      if(!B.__filter || A.text != B.__filter.text || A.invert != B.__filter.invert) {
         var G = B.__th.firstChild.firstChild;
         if(D && A.text) {
            if(!B.__filter) {
               G.className = "q5";
               ++(this.nFilters)}
            B.__filter = A}
         else {
            if(B.__filter) {
               G.className = "";
               --(this.nFilters)}
            B.__filter = null}
         this.updateFilters(1)}
      }
   };
Listview.headerOver = function(B, C, D) {
   var A = "";
   A += '<b class="q1">' + (C.tooltip ? C.tooltip : C.name) + "</b>";
   if(C.__filter) {
      A += "<br />" + sprintf((C.__filter.invert ? LANG.tooltip_colfilter2 : LANG.tooltip_colfilter1), C.__filter.text)}
   A += '<br /><span class="q2">' + LANG.tooltip_lvheader1 + "</span>";
   if(this.filtrable && (C.filtrable == null || C.filtrable)) {
      A += '<br /><span class="q2">' + (Browser.opera ? LANG.tooltip_lvheader3 : LANG.tooltip_lvheader2) + "</span>"}
   Tooltip.show(B, A, 0, 0, "q")};
Listview.extraCols = {
   cost : {
      id : "cost", name : LANG.cost, getValue : function(A) {
         return(A.cost[3] ? A.cost[3][0][1] : 0) || A.cost[2] || A.cost[1] || A.cost[0]}
      , compute : function(A, B) {
         Listview.funcBox.appendMoney(B, A.cost[0], null, A.cost[1], A.cost[2], A.cost[3])}
      , sortFunc : function(B, A, C) {
         var E = 0, D = 0;
         if(B.cost[3] != null) {
            array_walk(B.cost[3], function(F, G, G, H) {
               E += Math.pow(10, H) + F[1]}
            )}
         if(A.cost[3] != null) {
            array_walk(A.cost[3], function(F, G, G, H) {
               D += Math.pow(10, H) + F[1]}
            )}
         return strcmp(E, D) || strcmp(B.cost[2], A.cost[2]) || strcmp(B.cost[1], A.cost[1]) || strcmp(B.cost[0], A.cost[0])}
      }
   , count : {
      id : "count", name : LANG.count, width : "11%", value : "count", compute : function(B, C) {
         if(!(this._totalCount > 0 || B.outof > 0)) {
            return }
         if(B.outof) {
            var A = ce("div");
            A.className = "small q0";
            ae(A, ct(sprintf(LANG.lvdrop_outof, B.outof)));
            ae(C, A)}
         return B.count}
      , getVisibleText : function(A) {
         var B = A.count;
         if(A.outof) {
            B += " " + A.outof}
         return B}
      }
   , percent : {
      id : "percent", name : "%", width : "10%", value : "percent", compute : function(A, B) {
         if(A.count ==- 1) {
            return"??"}
         if(A.percent >= 1.95) {
            return A.percent.toFixed(0)}
         else {
            return parseFloat(A.percent.toFixed(1))}
         }
      , getVisibleText : function(A) {
         if(A.count ==- 1) {
            return"??"}
         if(A.percent >= 1.95) {
            return A.percent.toFixed(0)}
         else {
            return parseFloat(A.percent.toFixed(1))}
         }
      }
   , stock : {
      id : "stock", name : LANG.stock, width : "10%", value : "stock", compute : function(A, B) {
         if(A.stock > 0) {
            return A.stock}
         else {
            B.style.fontFamily = "Verdana, sans-serif";
            return String.fromCharCode(8734)}
         }
      , getVisibleText : function(A) {
         if(A.stock > 0) {
            return A.stock}
         else {
            return String.fromCharCode(8734) + " infinity"}
         }
      }
   };
Listview.funcBox = {
   createSimpleCol : function(C, D, A, B) {
      return {
         id : C, name : LANG[D], width : A, value : B}
      }
   , initLootTable : function(B) {
      var A;
      if(this._totalCount != null) {
         A = this._totalCount}
      else {
         A = B.outof}
      if(A == 0) {
         if(B.count !=- 1) {
            B.percent = B.count}
         else {
            B.percent = 0}
         }
      else {
         B.percent = B.count / A * 100}
      }
   , beforeUserComments : function() {
      if((g_user.roles & 26) != 0) {
         this.mode = 1;
         this.createCbControls = function(B) {
            var A = ce("input");
            A.type = "button";
            A.value = "Delete";
            A.onclick = (function() {
               var E = this.getCheckedRows(); if(!E.length) {
                  alert("No comments selected.")}
               else {
                  if(confirm("Are you sure that you want to delete " + (E.length == 1 ? "this comment" : "these " + E.length + " comments") + "?")) {
                     var C = ""; var D = 0; array_walk(E, function(F) {
                        if(F.purged == 0 && F.deleted == 0) {
                           F.deleted = 1; if(F.__tr != null) {
                              F.__tr.childNodes[2].lastChild.lastChild.firstChild.nodeValue = " (Deleted)"}
                           C += F.id + ","}
                        else {
                           if(F.purged == 1) {
                              ++D}
                           }
                        }
                     ); C = rtrim(C, ","); if(C != "") {
                        new Ajax("/?comment=delete&id=" + C + "&username=" + g_pageInfo.username)}
                     (Listview.cbSelect.bind(this, false))(); if(D > 0) {
                        alert("Purged comments cannot be deleted.\n\nA purged comment is a comment that has been\nautomatically removed from the site due to a negative rating.")}
                     }
                  }
               }
            ).bind(this);
            ae(B, A);
            if(g_user.roles & 26) {
               var A = ce("input");
               A.type = "button";
               A.value = "Undelete";
               A.onclick = (function() {
                  var D = this.getCheckedRows(); if(!D.length) {
                     alert("No comments selected.")}
                  else {
                     var C = ""; array_walk(D, function(E) {
                        if(E.deleted == 1) {
                           E.deleted = 0; if(E.__tr != null) {
                              E.__tr.childNodes[2].lastChild.lastChild.firstChild.nodeValue = ""}
                           C += E.id + ","}
                        }
                     ); C = rtrim(C, ","); if(C != "") {
                        new Ajax("/?comment=undelete&id=" + C + "&username=" + g_pageInfo.username)}
                     (Listview.cbSelect.bind(this, false))()}
                  }
               ).bind(this);
               ae(B, A)}
            }
         }
      }
   , assocArrCmp : function(C, B, A) {
      if(C == null) {
         return - 1}
      else {
         if(B == null) {
            return 1}
         }
      var F = Math.max(C.length, B.length);
      for(var E = 0; E < F; ++E) {
         if(C[E] == null) {
            return - 1}
         else {
            if(B[E] == null) {
               return 1}
            }
         var D = strcmp(A[C[E]], A[B[E]]);
         if(D != 0) {
            return D}
         }
      return 0}
   , location : function(E, F) {
      if(E.location == null) {
         return - 1}
      for(var D = 0, B = E.location.length; D < B; ++D) {
         if(D > 0) {
            ae(F, ct(LANG.comma))}
         var A = E.location[D];
         if(A ==- 1) {
            ae(F, ct(LANG.ellipsis))}
         else {
            var C = ce("a");
            C.className = "q1";
            C.href = "/?zone=" + A;
            ae(C, ct(g_zones[A]));
            ae(F, C)}
         }
      }
   , arrayText : function(B, D) {
      if(B == null) {
         return }
      var E = "";
      for(var C = 0, A = B.length; C < A; ++C) {
         if(C > 0) {
            E += " "}
         if(!D[B[C]]) {
            continue}
         E += D[B[C]]}
      return E}
   , createCenteredIcons : function(G, D, N, L) {
      if(G != null) {
         var J = ce("div"), A = ce("div");
         if(N) {
            var I = ce("div");
            I.style.position = "relative";
            I.style.width = "1px";
            var K = ce("div");
            K.className = "q0";
            K.style.position = "absolute";
            K.style.right = "2px";
            K.style.lineHeight = "26px";
            K.style.fontSize = "11px";
            K.style.whiteSpace = "nowrap";
            ae(K, ct(N));
            ae(I, K);
            ae(J, I)}
         var C = g_items;
         if(L == 1) {
            C = g_spells}
         for(var E = 0, H = G.length; E < H; ++E) {
            var M;
            if(G[E] == null) {
               M = ce("div");
               M.style.width = M.style.height = "26px"}
            else {
               var B, F;
               if(typeof G[E] == "object") {
                  B = G[E][0];
                  F = G[E][1]}
               else {
                  B = G[E]}
               M = C.createIcon(B, 0, F)}
            M.style.cssFloat = M.style.styleFloat = "left";
            ae(J, M)}
         J.style.margin = "0 auto";
         J.style.textAlign = "left";
         J.style.width = (26 * G.length) + "px";
         A.className = "clear";
         ae(D, J);
         ae(D, A);
         return true}
      }
   , getItemType : function(C, B, A) {
      if(A != null && g_item_subsubclasses[C] != null && g_item_subsubclasses[C][B] != null) {
         return {
            url : "/?items=" + C + "." + B + "." + A, text : g_item_subsubclasses[C][B][A]}
         }
      if(g_item_subclasses[C] != null) {
         return {
            url : "/?items=" + C + "." + B, text : g_item_subclasses[C][B]}
         }
      else {
         return {
            url : "/?items=" + C, text : g_item_classes[C]}
         }
      }
   , getQuestCategory : function(A) {
      if(A > 0) {
         return g_zones[A]}
      else {
         return g_quest_sorts[ - A]}
      }
   , getFactionCategory : function(B, A) {
      if(B) {
         return g_faction_categories[B]}
      else {
         return g_faction_categories[A]}
      }
   , createTextRange : function(B, A) {
      B |= 0;
      A |= 0;
      if(B > 1 || A > 1) {
         if(B != A && A > 0) {
            return B + "-" + A}
         else {
            return B + ""}
         }
      return null}
   , coReport : function(C, D, F) {
      if(!g_user.id ||!g_report_reasons[F]) {
         return }
      var A = "";
      if(F == 4) {
         A = prompt(LANG.prompt_details, "")}
      else {
         if(!confirm(sprintf((C == 0 ? LANG.confirm_report : LANG.confirm_report2), g_report_reasons[F]))) {
            return }
         }
      if(A != null) {
         var E = "/?report&type=" + C + "&typeid=" + D.id + "&reason=" + F;
         if(A) {
            E += "&reasonmore=" + urlencode(A)}
         new Ajax(E);
         var B = ce("span");
         ae(B, ct(LANG.lvcomment_reported));
         this.parentNode.replaceChild(B, this)}
      }
   , coReportClick : function(B, A, C) {
      this.menu = [[2, g_report_reasons[2], Listview.funcBox.coReport.bind(this, A, B, 2)], [1, g_report_reasons[1], Listview.funcBox.coReport.bind(this, A, B, 1)], [3, g_report_reasons[3], Listview.funcBox.coReport.bind(this, A, B, 3)], [4, g_report_reasons[4], Listview.funcBox.coReport.bind(this, A, B, 4)]];
      if(A == 1 && B.op && typeof g_pageInfo != "undefined" &&!g_pageInfo.sticky) {
         this.menu.splice(3, 0, [0, g_report_reasons[0], Listview.funcBox.coReport.bind(this, A, B, 0)])}
      (Menu.showAtCursor.bind(this, C))()}
   , coGetColor : function(C, A) {
      if(C.user && g_customColors[C.user]) {
         return" comment-" + g_customColors[C.user]}
      switch(A) {
         case - 1 : var B = C.divPost.childNodes[1].className.match(/comment-([a-z]+)/);
         if(B != null) {
            return" comment-" + B[1]}
         break;
         case 3 : case 4 : if(C.roles & 64) {
            return" comment-gold"}
         if(C.roles & 188) {
            return" comment-green"}
         break}
      if(C.roles & 2) {
         return" comment-blue"}
      else {
         if(C.rating >= 10) {
            return" comment-green"}
         else {
            if(C.rating < 0) {
               return" comment-bt"}
            }
         }
      return""}
   , coToggleVis : function(B) {
      this.firstChild.nodeValue = (g_toggleDisplay(B.divBody) ? LANG.lvcomment_hide : LANG.lvcomment_show);
      var A = B.divHeader.firstChild.lastChild;
      if(B.ratable) {
         A.style.display = ""}
      else {
         if(B.deleted || B.purged) {
            A.style.fontWeight = "normal";
            A.className = "q10";
            A.innerHTML = (B.deleted ? LANG.lvcomment_deleted : LANG.lvcomment_purged);
            A.style.display = ""}
         }
      g_toggleDisplay(B.divLinks);
      if(B.lastEdit != null) {
         g_toggleDisplay(B.divLastEdit)}
      }
   , coRate : function(D, B) {
      if(B == 0) {
         var A = 5;
         if(g_user.roles & 2) {
            A = 25}
         else {
            if(g_user.roles & 16) {
               A = 15}
            }
         var C = prompt(sprintf(LANG.prompt_customrating, A, A), 0);
         if(C == null) {
            return }
         else {
            C |= 0;
            if(C != 0 && Math.abs(C) <= A) {
               B = C}
            }
         if(B == 0) {
            return }
         }
      else {
         if(g_user.roles & 26) {
            B *= 5}
         }
      new Ajax("/?comment=rate&id=" + D.id + "&rating=" + B);
      D.rating += B;
      _ = D.divHeader.firstChild;
      _ = _.childNodes[_.childNodes.length - 3];
      _.lastChild.firstChild.nodeValue = (D.rating > 0 ? "+" : "") + D.rating;
      Tooltip.hide();
      de(_.nextSibling);
      de(_.nextSibling)}
   , coDelete : function(A) {
      if(A.purged) {
         alert(LANG.message_cantdeletecomment)}
      else {
         if(confirm(LANG.confirm_deletecomment)) {
            new Ajax("/?comment=delete&id=" + A.id);
            this.deleteRows([A])}
         }
      }
   , coDetach : function(A) {
      if(A.replyTo == 0) {
         alert(LANG.message_cantdetachcomment)}
      else {
         if(confirm(LANG.confirm_detachcomment)) {
            new Ajax("/?comment=detach&id=" + A.id);
            A.replyTo = 0;
            alert(LANG.message_commentdetached)}
         }
      }
   , coEdit : function(G, E) {
      G.divBody.style.display = "none";
      G.divLinks.firstChild.style.display = "none";
      var F = ce("div");
      F.className = "comment-edit";
      G.divEdit = F;
      if(E ==- 1) {
         if(g_users[G.user] != null) {
            G.roles = g_users[G.user].roles}
         }
      var C = Listview.funcBox.coEditAppend(F, G, E);
      var B = ce("div");
      B.className = "comment-edit-buttons";
      var D = ce("input");
      D.type = "button";
      D.value = LANG.compose_save;
      D.onclick = Listview.funcBox.coEditButton.bind(D, G, true, E);
      ae(B, D);
      ae(B, ct(" "));
      D = ce("input");
      D.type = "button";
      D.value = LANG.compose_cancel;
      D.onclick = Listview.funcBox.coEditButton.bind(D, G, false, E);
      ae(B, D);
      ae(F, B);
      var A = F;
      if(Browser.ie6) {
         A = ce("div");
         A.style.width = "99%";
         ae(A, F)}
      if(E ==- 1) {
         G.divPost.insertBefore(A, G.divBody.nextSibling)}
      else {
         G.__div.insertBefore(A, G.divBody.nextSibling)}
      C.focus()}
   , coEditAppend : function(L, C, K) {
      var F = Listview.funcBox.coGetCharLimit(K);
      if(K == 1 || K == 3 || K == 4) {
         C.user = g_user.name;
         C.roles = g_user.roles;
         C.rating = 1}
      else {
         if(K == 2) {
            C.roles = g_user.roles;
            C.rating = 1}
         }
      if(K ==- 1 || K == 0) {
         var J = ce("div");
         J.className = "comment-edit-modes";
         ae(J, ct(LANG.compose_mode));
         var P = ce("a");
         P.className = "selected";
         P.onclick = Listview.funcBox.coModeLink.bind(P, 1, K, C);
         P.href = "javascript:;";
         ae(P, ct(LANG.compose_edit));
         ae(J, P);
         ae(J, ct("|"));
         var N = ce("a");
         N.onclick = Listview.funcBox.coModeLink.bind(N, 2, K, C);
         N.href = "javascript:;";
         ae(N, ct(LANG.compose_preview));
         ae(J, N);
         ae(L, J)}
      var A = ce("div");
      A.style.display = "none";
      A.className = "comment-body" + Listview.funcBox.coGetColor(C, K);
      ae(L, A);
      var H = ce("div");
      H.className = "comment-edit-body";
      var G = ce("div");
      G.className = "toolbar";
      var I = ce("textarea");
      I.className = "comment-editbox";
      I.rows = 10;
      I.value = C.body;
      switch(K) {
         case 1 : I.name = "commentbody";
         I.onfocus = g_revealCaptcha;
         break;
         case 2 : I.name = "desc";
         I.originalValue = C.body;
         break;
         case 3 : I.name = "body";
         I.onfocus = g_revealCaptcha;
         break;
         case 4 : I.name = "sig";
         I.originalValue = C.body;
         I.rows = (Browser.gecko ? 2 : 3);
         I.style.height = "auto";
         break}
      if(K !=- 1 && K != 0) {
         var E = ce("h3"), U = ce("a"), O = ce("div"), T = ce("div");
         var D = Listview.funcBox.coLivePreview.bind(I, C, K, O);
         if(C.body) {
            U.className = "disclosure-off";
            O.style.display = "none"}
         else {
            U.className = "disclosure-on"}
         ae(U, ct(LANG.compose_livepreview));
         ae(E, U);
         U.href = "javascript:;";
         U.onclick = function() {
            D(1);
            U.className = "disclosure-" + (g_toggleDisplay(O) ? "on" : "off")};
         ns(U);
         E.className = "first";
         T.className = "pad";
         ae(A, E);
         ae(A, O);
         ae(A, T);
         g_onAfterTyping(I, D, 50);
         aE(I, "focus", function() {
            D(); A.style.display = ""; if(K != 4) {
               I.style.height = "22em"}
            }
         )}
      else {
         if(K != 4) {
            aE(I, "focus", function() {
               I.style.height = "22em"}
            )}
         }
      var S = [ {
         id : "b", title : LANG.markup_b, pre : "[b]", post : "[/b]"}
      , {
         id : "i", title : LANG.markup_i, pre : "[i]", post : "[/i]"}
      , {
         id : "u", title : LANG.markup_u, pre : "[u]", post : "[/u]"}
      , {
         id : "s", title : LANG.markup_s, pre : "[s]", post : "[/s]"}
      , {
         id : "small", title : LANG.markup_small, pre : "[small]", post : "[/small]"}
      , {
         id : "url", title : LANG.markup_url, onclick : function() {
            var W = prompt(LANG.prompt_linkurl, "http://");
            if(W) {
               g_insertTag(I, "[url=" + W + "]", "[/url]")}
            }
         }
      , {
         id : "quote", title : LANG.markup_quote, pre : "[quote]", post : "[/quote]"}
      , {
         id : "code", title : LANG.markup_code, pre : "[code]", post : "[/code]"}
      , {
         id : "ul", title : LANG.markup_ul, pre : "[ul]\n[li]", post : "[/li]\n[/ul]", rep : function(W) {
            return W.replace(/\n/g,"[/li]\n[li]")}},{id:"ol",title:LANG.markup_ol,pre:"[ol]\n[li]",post:"[/li]\n[/ol]",rep:function(W){return W.replace(/\n/g,"[/li]\n[li]")}},{id:"li",title:LANG.markup_li,pre:"[li]",post:"[/li]"}];for(var Q=0,R=S.length;Q<R;++Q){var B=S[Q];if(K==4&&B.id=="quote"){break}var M=ce("button");var V=ce("img");M.setAttribute("type","button");M.title=B.title;if(B.onclick!=null){M.onclick=B.onclick}else{M.onclick=g_insertTag.bind(0,I,B.pre,B.post,B.rep)}V.src="http ://static.wowhead.com/images/pixel.gif";V.className="toolbar-"+B.id;ae(M,V);ae(G,M)}ae(H,G);ae(H,I);ae(H,ce("br"));if(K==4){ae(H,ct(sprintf(LANG.compose_limit2,F,3)))}else{ae(H,ct(sprintf(LANG.compose_limit,F)))}ae(L,H);return I},coLivePreview:function(E,F,A,B){if(B!=1&&A.style.display=="none"){return }var C=this,I=Listview.funcBox.coGetCharLimit(F),G=(C.value.length>I?C.value.substring(0,I):C.value);if(F==4){var H;if((H=G.indexOf("\n"))!=-1&&(H=G.indexOf("\n",H+1))!=-1&&(H=G.indexOf("\n",H+1))!=-1){G=G.substring(0,H)}}var D=Markup.toHtml(G,{mode:Markup.MODE_COMMENT,roles:E.roles});if(D){A.innerHTML=D}else{A.innerHTML='<span class="q6">...</span>'}},coEditButton:function(F,E,C){if(E){var A=gE(F.divEdit,"textarea")[0];if(!Listview.funcBox.coValidate(A,C)){return }if(A.value!=F.body){var D=0;if(F.lastEdit!=null){D=F.lastEdit[1]}++D;F.lastEdit=[g_serverTime,D,g_user.name];Listview.funcBox.coUpdateLastEdit(F);var B=Listview.funcBox.coGetCharLimit(C);F.divBody.innerHTML=Markup.toHtml((A.value.length>B?A.value.substring(0,B):A.value),{mode:Markup.MODE_COMMENT,roles:F.roles});F.body=A.value;if(C==-1){new Ajax("/?forums=editpost&id="+F.id,{method:"POST",params:"body="+urlencode(F.body)})}else{new Ajax("/?comment=edit&id="+F.id,{method:"POST",params:"body="+urlencode(F.body)})}}}F.divBody.style.display="";F.divLinks.firstChild.style.display="";de(F.divEdit);F.divEdit=null},coGetCharLimit:function(A){switch(A){case 0:case 1:case 2:return 7500;case 4:return 250;default:return 15000}},coModeLink:function(E,A,D){var J=Listview.funcBox.coGetCharLimit(E);var B=Markup.MODE_COMMENT;array_walk(gE(this.parentNode,"a"),function(K){K.className=""});this.className="selected";var C=gE(this.parentNode.parentNode,"textarea")[0],I=C.parentNode,F=I.previousSibling;if(A==4){B=Markup.MODE_SIGNATURE}switch(E){case 1:I.style.display="";F.style.display="none";I.firstChild.focus();break;case 2:I.style.display="none";var G=(C.value.length>J?C.value.substring(0,J):C.value);if(A==4){var H;if((H=G.indexOf("\n"))!=-1&&(H=G.indexOf("\n",H+1))!=-1&&(H=G.indexOf("\n",H+1))!=-1){G=G.substring(0,H)}}F.innerHTML=Markup.toHtml(G,{mode:B,roles:D.roles});F.style.display="";break}},coReply:function(B){document.forms.addcomment.elements.replyto.value=B.replyTo;var A=ge("gjkdlfgkjh436");gE(A,"span")[0].innerHTML=B.user;A.style.display="";co_addYourComment()},coValidate:function(A,C){C|=0;if(C==1||C==-1){if(trim(A.value).length<1){alert(LANG.message_forumposttooshort);return false}}else{if(trim(A.value).length<10){alert(LANG.message_commenttooshort);return false}}var B=Listview.funcBox.coGetCharLimit(C);if(A.value.length>B){if(!confirm(sprintf(C==1?LANG.confirm_forumposttoolong:LANG.confirm_commenttoolong,B,A.value.substring(B-30,B)))){return false}}return true},coCustomRatingOver:function(A){Tooltip.showAtCursor(A,LANG.tooltip_customrating,0,0,"q")},coPlusRatingOver:function(A){Tooltip.showAtCursor(A,LANG.tooltip_uprate,0,0,"q2")},coMinusRatingOver:function(A){Tooltip.showAtCursor(A,LANG.tooltip_downrate,0,0,"q10")},coSortDate:function(A){A.nextSibling.nextSibling.className="";A.className="selected";this.mainDiv.className="listview-aci";this.setSort([1],true,false)},coSortHighestRatedFirst:function(A){A.previousSibling.previousSibling.className="";A.className="selected";this.mainDiv.className="";this.setSort([-3,2],true,false)},coUpdateLastEdit:function(F){var B=F.divLastEdit;if(!B){return }if(F.lastEdit!=null){var E=F.lastEdit;B.childNodes[1].firstChild.nodeValue=E[2];B.childNodes[1].href="/?user="+E[2];var D=new Date(E[0]);var A=(g_serverTime-D)/1000;if(B.childNodes[3].firstChild){de(B.childNodes[3].firstChild)}Listview.funcBox.coFormatDate(B.childNodes[3],A,D);var C="";if(F.rating!=null){C+=LANG.lvcomment_patch1+g_getPatchVersion(D)+LANG.lvcomment_patch2}if(E[1]>1){C+=LANG.dash+sprintf(LANG.lvcomment_nedits,E[1])}B.childNodes[4].nodeValue=C;B.style.display=""}else{B.style.display="none"}},coFormatDate:function(E,A,D,F,G){var C;if(A<2592000){txt=sprintf(LANG.date_ago,g_formatTimeElapsed(A));var B=D;B.setTime(B.getTime()+(g_localTime-g_serverTime));E.style.cursor="help";E.title=B.toLocaleString()}else{txt=LANG.date_on+g_formatDateSimple(D,F)}if(G==1){txt=txt.substr(0,1).toUpperCase()+txt.substr(1)}C=ct(txt);ae(E,C)},ssCellOver:function(){this.className="screenshot-caption-over"},ssCellOut:function(){this.className="screenshot-caption"},ssCellClick:function(B,D){D=$E(D);if(D.shiftKey||D.ctrlKey){return }var A=0,C=D._target;while(C&&A<3){if(C.nodeName=="A"){return }if(C.nodeName=="IMG"){break}C=C.parentNode}ScreenshotViewer.show({screenshots:this.data,pos:B})},moneyHonorOver:function(A){Tooltip.showAtCursor(A,LANG.tooltip_honorpoints,0,0,"q")},moneyArenaOver:function(A){Tooltip.showAtCursor(A,LANG.tooltip_arenapoints,0,0,"q")},moneyAchievementOver:function(A){Tooltip.showAtCursor(A,LANG.tooltip_achievementpoints,0,0,"q")},appendMoney:function(E,A,F,J,L,B,K){var I,H=0;if(A>=10000){H=1;I=ce("span");I.className="moneygold";ae(I,ct(Math.floor(A/10000)));ae(E,I);A%=10000}if(A>=100){if(H){ae(E,ct(" "))}else{H=1}I=ce("span");I.className="moneysilver";ae(I,ct(Math.floor(A/100)));ae(E,I);A%=100}if(A>=1||F!=null){if(H){ae(E,ct(" "))}else{H=1}I=ce("span");I.className="moneycopper";ae(I,ct(A));ae(E,I)}if(J!=null&&J!=0){if(H){ae(E,ct(" "))}else{H=1}I=ce("span");I.className="money"+(J<0?"horde":"alliance")+" tip";I.onmouseover=Listview.funcBox.moneyHonorOver;I.onmousemove=Tooltip.cursorUpdate;I.onmouseout=Tooltip.hide;ae(I,ct(number_format(Math.abs(J))));ae(E,I)}if(L>=1){if(H){ae(E,ct(" "))}else{H=1}I=ce("span");I.className="moneyarena tip";I.onmouseover=Listview.funcBox.moneyArenaOver;I.onmousemove=Tooltip.cursorUpdate;I.onmouseout=Tooltip.hide;ae(I,ct(number_format(L)));ae(E,I)}if(B!=null){for(var C=0;C<B.length;++C){if(H){ae(E,ct(" "))}else{H=1}var G=B[C][0];var D=B[C][1];I=ce("a");I.href="./?i="+G;I.className="moneyitem";I.style.backgroundImage="url(http://static.wowhead.com/images/icons/tiny/"+g_items.getIcon(G).toLowerCase()+".gif)";ae(I,ct(D));ae(E,I)}}if(K!=null){if(H){ae(E,ct(" "))}else{H=1}I=ce("span");I.className="moneyachievement tip";I.onmouseover=Listview.funcBox.moneyAchievementOver;I.onmousemove=Tooltip.cursorUpdate;I.onmouseout=Tooltip.hide;ae(I,ct(number_format(K)));ae(E,I)}},getUpperSource:function(A,B){switch(A){case 2:if(B.z){return LANG.source_zonedrop}break;case 4:return LANG.source_quests;case 5:return LANG.source_vendors}return g_sources[A]},getLowerSource:function(C,D,B){switch(C){case 3:if(D.p&&g_sources_pvp[D.p]){return{text:g_sources_pvp[D.p]}}break}switch(B){case 0:case 1:case 2:if(D.z){var A={url:"/?zone="+D.z,text:g_zones[D.z]};if(D.t&&C==5){A.pretext=LANG.lvitem_vendorin}if(D.dd){if(D.dd==1){A.posttext=LANG.lvitem_normal}else{if(D.dd==2){A.posttext=LANG.lvitem_heroic}}}return A}break;case 5:return{url:"/?quests="+D.c2+"."+D.c,text:Listview.funcBox.getQuestCategory(D.c)};break;case 6:if(D.c&&D.s){return{url:"/?spells="+D.c+"."+D.s,text:g_spell_skills[D.s]}}else{return{url:"/?spells=0",text:"??"}}break}}};Listview.templates={faction:{sort:[1],nItemsPerPage:-1,searchable:1,filtrable:1,columns:[{id:"name",name:LANG.name,type:"text",align:"left",value:"name",compute:function(C,D){var A=ce("a");A.style.fontFamily="Verdana, sans-serif";A.href=this.template.getItemLink(C);ae(A,ct(C.name));if(C.expansion){var B=ce("span");B.className=(C.expansion==1?"bc-icon":"wotlk-icon");ae(B,A);ae(D,B)}else{ae(D,A)}},getVisibleText:function(A){var B=A.name;if(A.expansion==1){B+=" bc"}else{if(A.expansion==2){B+="wotlk wrath"}}return B}},{id:"side",name:LANG.side,type:"text",width:"10%",compute:function(B,C){if(B.side){var A=ce("span");A.className=(B.side==1?"alliance-icon":"horde-icon");ae(A,ct(g_sides[B.side]));ae(C,A)}},getVisibleText:function(A){if(A.side){return g_sides[A.side]}},sortFunc:function(B,A,C){return strcmp(g_sides[B.side],g_sides[A.side])}},{id:"category",name:LANG.category,type:"text",width:"16%",compute:function(C,D){if(C.category2!=null){D.className="small q1";var A=ce("a"),B="/?factions="+C.category2;if(C.category){B+="."+C.category}A.href=B;ae(A,ct(Listview.funcBox.getFactionCategory(C.category,C.category2)));ae(D,A)}},getVisibleText:function(A){return Listview.funcBox.getFactionCategory(A.category,A.category2)},sortFunc:function(B,A,D){var C=Listview.funcBox.getFactionCategory;return strcmp(C(B.category,B.category2),C(A.category,A.category2))}}],getItemLink:function(A){return"/?faction="+A.id}},item:{sort:[1],searchable:1,filtrable:1,columns:[{id:"name",name:LANG.name,type:"text",align:"left",span:2,value:"name",compute:function(J,B,G){var C=ce("td");C.style.width="1px";C.style.padding="0";C.style.borderRight="none";var D=null,I=null;if(J.stack!=null){D=Listview.funcBox.createTextRange(J.stack[0],J.stack[1])}if(J.avail!=null){I=J.avail}ae(C,g_items.createIcon(J.id,1,D,I));ae(G,C);B.style.borderLeft="none";var H=ce("a");H.className="q"+(7-parseInt(J.name.charAt(0)));H.style.fontFamily="Verdana, sans-serif";H.href=this.template.getItemLink(J);ae(H,ct(J.name.substring(1)));ae(B,H);if(typeof fi_nExtraCols=="number"&&fi_nExtraCols>=5){if(J.source!=null&&J.source.length==1){var E=ce("div");E.className="small2";var A=(J.sourcemore?J.sourcemore[0]:{});var F=0;if(A.t){F=A.t;var H=ce("a");if(A.q!=null){H.className="q"+A.q}else{H.className="q1"}H.href="/?"+g_types[A.t]+"="+A.ti;if(A.n.length<=30){ae(H,ct(A.n))}else{H.title=A.n;ae(H,ct(trim(A.n.substr(0,27))+"..."))}ae(E,H)}else{ae(E,ct(Listview.funcBox.getUpperSource(J.source[0],A)))}ls=Listview.funcBox.getLowerSource(J.source[0],A,F);if(ls!=null){ae(E,ct(LANG.hyphen));if(ls.pretext){ae(E,ct(ls.pretext))}if(ls.url){var H=ce("a");H.className="q1";H.href=ls.url;ae(H,ct(ls.text));ae(E,H)}else{ae(E,ct(ls.text))}if(ls.posttext){ae(E,ct(ls.posttext))}}ae(B,E)}}},getVisibleText:function(B){var D=B.name.substring(1);if(typeof fi_nExtraCols=="number"&&fi_nExtraCols>=5){if(B.source!=null&&B.source.length==1){var C=(B.sourcemore?B.sourcemore[0]:{});var A=0;if(C.t){A=C.t;D+=" "+C.n}else{D+=" "+Listview.funcBox.getUpperSource(B.source[0],C)}ls=Listview.funcBox.getLowerSource(B.source[0],C,A);if(ls!=null){if(ls.pretext){D+=" "+ls.pretext}D+=" "+ls.text;if(ls.posttext){D+=" "+ls.posttext}}}}return D}},{id:"level",name:LANG.level,value:"level"},{id:"reqlevel",name:LANG.req,tooltip:LANG.tooltip_reqlevel,value:"reqlevel",compute:function(A,B){if(A.reqlevel>1){return A.reqlevel}}},{id:"dps",name:LANG.dps,value:"dps",compute:function(A,B){return(A.dps||0).toFixed(1)},hidden:true},{id:"speed",name:LANG.speed,value:"speed",compute:function(A,B){return(A.speed||0).toFixed(2)},hidden:true},{id:"armor",name:LANG.armor,value:"armor",compute:function(A,B){if(A.armor>0){return A.armor}},hidden:true},{id:"slot",name:LANG.slot,type:"text",compute:function(A,B){nw(B);return g_item_slots[A.slot]},getVisibleText:function(A){return g_item_slots[A.slot]},sortFunc:function(B,A,C){return strcmp(g_item_slots[B.slot],g_item_slots[A.slot])},hidden:true},{id:"slots",name:LANG.slots,value:"nslots",hidden:true},{id:"skill",name:LANG.skill,value:"skill",hidden:true},{id:"glyph",name:LANG.glyphtype,type:"text",value:"glyph",compute:function(A,B){if(A.glyph){return g_item_glyphs[A.glyph]}},getVisibleText:function(A){return g_item_glyphs[A.glyph]},sortFunc:function(B,A,C){return strcmp(g_item_glyphs[B.glyph],g_item_glyphs[A.glyph])},hidden:true},{id:"source",name:LANG.source,type:"text",compute:function(I,C){if(I.source!=null){if(I.source.length==1){nw(C);var B=(I.sourcemore?I.sourcemore[0]:{});var F=0;if(B.t){F=B.t;var G=ce("a");if(B.q!=null){G.className="q"+B.q}else{G.className="q1"}G.href="/?"+g_types[B.t]+"="+B.ti;if(B.n.length<=20){ae(G,ct(B.n))}else{G.title=B.n;ae(G,ct(trim(B.n.substr(0,17))+"..."))}ae(C,G)}else{ae(C,ct(Listview.funcBox.getUpperSource(I.source[0],B)))}ls=Listview.funcBox.getLowerSource(I.source[0],B,F);if(ls!=null){var A=ce("div");A.className="small2";if(ls.pretext){ae(A,ct(ls.pretext))}if(ls.url){var G=ce("a");G.className="q1";G.href=ls.url;ae(G,ct(ls.text));ae(A,G)}else{ae(A,ct(ls.text))}if(ls.posttext){ae(A,ct(ls.posttext))}ae(C,A)}}else{var H="";for(var D=0,E=I.source.length;D<E;++D){if(D>0){H+=LANG.comma}H+=g_sources[I.source[D]]}return H}}},getVisibleText:function(B){if(B.source!=null){if(B.source.length==1){var D="";var C=(B.sourcemore?B.sourcemore[0]:{});var A=0;if(C.t){A=C.t;D+=" "+C.n}else{D+=" "+Listview.funcBox.getUpperSource(B.source[0],C)}ls=Listview.funcBox.getLowerSource(B.source[0],C,A);if(ls!=null){if(ls.pretext){D+=" "+ls.pretext}D+=" "+ls.text;if(ls.posttext){D+=" "+ls.posttext}}return D}else{return Listview.funcBox.arrayText(B.source,g_sources)}}},sortFunc:function(D,B){var E=Listview.funcBox.assocArrCmp(D.source,B.source,g_sources);if(E!=0){return E}var C=(D.sourcemore&&D.source.length==1?D.sourcemore[0].n:null),A=(B.sourcemore&&B.source.length==1?B.sourcemore[0].n:null);return strcmp(C,A)}},{id:"type",name:LANG.type,type:"text",compute:function(C,D){D.className="small q1";nw(D);var A=ce("a");var B=Listview.funcBox.getItemType(C.classs,C.subclass,C.subsubclass);A.href=B.url;ae(A,ct(B.text));ae(D,A)},getVisibleText:function(A){return Listview.funcBox.getItemType(A.classs,A.subclass,A.subsubclass).text},sortFunc:function(B,A,D){var C=Listview.funcBox.getItemType;return strcmp(C(B.classs,B.subclass,B.subsubclass).text,C(A.classs,A.subclass,A.subsubclass).text)}}],getItemLink:function(A){return"./?i="+A.id},onBeforeCreate:function(){var B=false;for(var C=0,A=this.data.length;C<A;++C){var D=this.data[C];if(D.slot>0&&D.slot!=18){++B}else{D.__nochk=1}}if(B>0){this.mode=1;this._nComparable=B}},createCbControls:function(C,B,E){if(!E&&this._nComparable<15){C.style.display="none";B.innerHTML="&nbsp;";return }var D=ce("input"),A=ce("input");C.className="listview-withselected"+(E?"":"2");D.type=A.type="button";D.value=LANG.button_compare;A.value=LANG.button_deselect;D.onclick=this.template.compareItems.bind(this);A.onclick=Listview.cbSelect.bind(this,false);ae(C,D);ae(C,A);if(E&&(this.note||this.createNote)){C.style.marginTop="6px"}C.style.paddingBottom="0";B.style.display="none"},compareItems:function(){var B=this.getCheckedRows();if(!B.length){return }var A="";array_walk(B,function(C){A+=C.id+";"});su_addToSaved(rtrim(A,";"))}},itemset:{sort:[1],nItemsPerPage:75,searchable:1,filtrable:1,columns:[{id:"name",name:LANG.name,type:"text",align:"left",value:"name",compute:function(B,D){var A=ce("a");A.className="q"+(7-parseInt(B.name.charAt(0)));A.style.fontFamily="Verdana, sans-serif";A.href=this.template.getItemLink(B);ae(A,ct(B.name.substring(1)));ae(D,A);if(B.note){var C=ce("div");C.className="small";ae(C,ct(g_itemset_notes[B.note]));ae(D,C)}},getVisibleText:function(A){var B=A.name.substring(1);if(A.note){B+=" "+g_itemset_notes[A.note]}return B}},{id:"level",name:LANG.level,type:"range",getMinValue:function(A){return A.minlevel},getMaxValue:function(A){return A.maxlevel},compute:function(A,B){if(A.minlevel>0&&A.maxlevel>0){if(A.minlevel!=A.maxlevel){return A.minlevel+LANG.hyphen+A.maxlevel}else{return A.minlevel}}else{return -1}},sortFunc:function(B,A,C){if(C>0){return strcmp(B.minlevel,A.minlevel)||strcmp(B.maxlevel,A.maxlevel)}else{return strcmp(B.maxlevel,A.maxlevel)||strcmp(B.minlevel,A.minlevel)}}},{id:"pieces",name:LANG.pieces,getValue:function(A){return A.pieces.length},compute:function(A,B){B.style.padding="0";Listview.funcBox.createCenteredIcons(A.pieces,B)},sortFunc:function(B,A){var D=(B.pieces!=null?B.pieces.length:0);var C=(A.pieces!=null?A.pieces.length:0);return strcmp(D,C)}},{id:"type",name:LANG.type,type:"text",compute:function(A,B){return g_itemset_types[A.type]},sortFunc:function(B,A,C){return strcmp(g_itemset_types[B.type],g_itemset_types[A.type])}},{id:"classes",name:LANG.classes,type:"text",compute:function(C,E){if(C.classes!=null){var D="";for(var B=0,A=C.classes.length;B<A;++B){if(B>0){D+=LANG.comma}D+=g_chr_classes[C.classes[B]]}return D}},sortFunc:function(B,A,C){return Listview.funcBox.assocArrCmp(B.classes,A.classes,g_chr_classes)}}],getItemLink:function(A){return"/?itemset="+A.id}},npc:{sort:[1],nItemsPerPage:100,searchable:1,filtrable:1,columns:[{id:"name",name:LANG.name,type:"text",align:"left",value:"name",compute:function(B,D){var A=ce("a");A.style.fontFamily="Verdana, sans-serif";A.href=this.template.getItemLink(B);ae(A,ct(B.name));ae(D,A);if(B.tag!=null){var C=ce("div");C.className="small";ae(C,ct("<"+B.tag+">"));ae(D,C)}},getVisibleText:function(A){var B=A.name;if(A.tag){B+=" <"+A.tag+">"}return B}},{id:"level",name:LANG.level,type:"range",width:"10%",getMinValue:function(A){return A.minlevel},getMaxValue:function(A){return A.maxlevel},compute:function(A,C){if(A.classification){var B=ce("div");B.className="small";ae(B,ct(g_npc_classifications[A.classification]));ae(C,B)}if(A.classification==3){return"??"}if(A.minlevel>0&&A.maxlevel>0){if(A.minlevel!=A.maxlevel){return A.minlevel+LANG.hyphen+A.maxlevel}else{return A.minlevel}}return -1},getVisibleText:function(A){var B="";if(A.classification){B+=" "+g_npc_classifications[A.classification]}if(A.minlevel>0&&A.maxlevel>0){B+=" ";if(A.minlevel!=A.maxlevel){B+=A.minlevel+LANG.hyphen+A.maxlevel}else{B+=A.minlevel}}return B},sortFunc:function(B,A,C){if(C>0){return strcmp(B.minlevel,A.minlevel)||strcmp(B.maxlevel,A.maxlevel)||strcmp(B.classification,A.classification)}else{return strcmp(B.maxlevel,A.maxlevel)||strcmp(B.minlevel,A.minlevel)||strcmp(B.classification,A.classification)}}},{id:"location",name:LANG.location,type:"text",compute:function(A,B){return Listview.funcBox.location(A,B)},getVisibleText:function(A){return Listview.funcBox.arrayText(A.location,g_zones)},sortFunc:function(B,A,C){return Listview.funcBox.assocArrCmp(B.location,A.location,g_zones)}},{id:"react",name:LANG.react,type:"text",width:"10%",value:"react",filtrable:0,compute:function(B,F){if(B.react==null){return -1}var C=[LANG.lvnpc_alliance,LANG.lvnpc_horde];var E=0;for(var A=0;A<2;++A){if(B.react[A]!=null){if(E++>0){ae(F,ct(" "))}var D=ce("span");D.className=(B.react[A]<0?"q10":(B.react[A]>0?"q2":"q"));ae(D,ct(C[A]));ae(F,D)}}}},{id:"skin",name:LANG.skin,type:"text",value:"skin",compute:function(B,C){if(B.skin){var A=ce("a");A.className="q1";A.href="/?npcs&filter=cr=35;crs=0;crv="+B.skin;ae(A,ct(B.skin));ae(C,A)}},hidden:1},{id:"petfamily",name:LANG.petfamily,type:"text",width:"12%",compute:function(B,C){C.className="q1";var A=ce("a");A.href="/?pet="+B.family;ae(A,ct(g_pet_families[B.family]));ae(C,A)},getVisibleText:function(A){return g_pet_families[A.family]},sortFunc:function(B,A,C){return strcmp(g_pet_families[B.family],g_pet_families[A.family])},hidden:1},{id:"type",name:LANG.type,type:"text",width:"12%",compute:function(B,C){C.className="small q1";var A=ce("a");A.href="/?npcs="+B.type;ae(A,ct(g_npc_types[B.type]));ae(C,A)},getVisibleText:function(A){return g_npc_types[A.type]},sortFunc:function(B,A,C){return strcmp(g_npc_types[B.type],g_npc_types[A.type])}}],getItemLink:function(A){return"/?npc="+A.id}},object:{sort:[1],nItemsPerPage:100,searchable:1,filtrable:1,columns:[{id:"name",name:LANG.name,type:"text",align:"left",value:"name",compute:function(B,C){var A=ce("a");A.style.fontFamily="Verdana, sans-serif";A.href=this.template.getItemLink(B);ae(A,ct(B.name));ae(C,A)}},{id:"location",name:LANG.location,type:"text",compute:function(A,B){return Listview.funcBox.location(A,B)},getVisibleText:function(A){return Listview.funcBox.arrayText(A.location,g_zones)},sortFunc:function(B,A,C){return Listview.funcBox.assocArrCmp(B.location,A.location,g_zones)}},{id:"skill",name:LANG.skill,width:"10%",value:"skill",hidden:true},{id:"type",name:LANG.type,type:"text",width:"12%",compute:function(B,C){C.className="small q1";var A=ce("a");A.href="/?objects="+B.type;ae(A,ct(g_object_types[B.type]));ae(C,A)},getVisibleText:function(A){return g_object_types[A.type]},sortFunc:function(B,A,C){return strcmp(g_object_types[B.type],g_object_types[A.type])}}],getItemLink:function(A){return"/?object="+A.id}},quest:{sort:[1,2],nItemsPerPage:100,searchable:1,filtrable:1,columns:[{id:"name",name:LANG.name,type:"text",align:"left",value:"name",compute:function(B,C){var A=ce("a");A.style.fontFamily="Verdana, sans-serif";A.href=this.template.getItemLink(B);ae(A,ct(B.name));ae(C,A)}},{id:"level",name:LANG.level,width:"7%",value:"level",compute:function(A,C){if(A.type||A.daily){var B=ce("div");B.className="small";nw(B);if(A.type&&A.daily){ae(B,ct(sprintf(LANG.lvquest_daily,g_quest_types[A.type])))}else{if(A.daily){ae(B,ct(LANG.daily))}else{if(A.type){ae(B,ct(g_quest_types[A.type]))}}}ae(C,B)}return A.level},getVisibleText:function(A){var B="";if(A.type){B+=" "+g_quest_types[A.type]}if(A.daily){B+=" "+LANG.daily}if(A.level){B+=" "+A.level}return B},sortFunc:function(B,A,C){return strcmp(B.level,A.level)||strcmp(B.type,A.type)}},{id:"reqlevel",name:LANG.req,tooltip:LANG.tooltip_reqlevel,width:"7%",value:"reqlevel"},{id:"side",name:LANG.side,type:"text",width:"10%",compute:function(A,C){if(A.side){var B=ce("span");if(A.side==1){B.className="alliance-icon"}else{if(A.side==2){B.className="horde-icon"}}ae(B,ct(g_sides[A.side]));ae(C,B)}else{return -1}},getVisibleText:function(A){if(A.side){return g_sides[A.side]}},sortFunc:function(B,A,C){return strcmp(g_sides[B.side],g_sides[A.side])}},{id:"rewards",name:LANG.rewards,filtrable:0,width:"25%",compute:function(B,F){var A=(B.itemchoices!=null||B.itemrewards!=null);if(A){F.style.padding="0";var E,D;if(B.itemchoices&&B.itemchoices.length>1){E=LANG.lvquest_pickone;if(B.itemrewards&&B.itemrewards.length>0){D=LANG.lvquest_alsoget}}Listview.funcBox.createCenteredIcons(B.itemchoices,F,E);Listview.funcBox.createCenteredIcons(B.itemrewards,F,D)}if(B.xp>0||B.money>0){var C=ce("div");if(A){C.style.padding="4px"}if(B.xp>0){ae(C,ct(sprintf(LANG.lvquest_xp,B.xp)+(B.money>0?" + ":"")))}if(B.money>0){Listview.funcBox.appendMoney(C,B.money)}ae(F,C)}},getVisibleText:function(A){var B="";if(A.itemchoices&&A.itemchoices.length){B+=" "+LANG.lvquest_pickone;if(A.itemrewards&&A.itemrewards.length){B+=" "+LANG.lvquest_alsoget}}if(A.xp>0){B+=" "+sprintf(LANG.lvquest_xp,A.xp)}return B},sortFunc:function(B,A,C){var E=(B.itemchoices!=null?B.itemchoices.length:0)+(B.itemrewards!=null?B.itemrewards.length:0);var D=(A.itemchoices!=null?A.itemchoices.length:0)+(A.itemrewards!=null?A.itemrewards.length:0);return strcmp(E,D)||strcmp((B.xp|0)+(B.money|0),(A.xp|0)+(A.money|0))}},{id:"reputation",name:LANG.reputation,width:"14%",value:"id",hidden:true},{id:"category",name:LANG.category,type:"text",width:"16%",compute:function(B,C){if(B.category!=0){C.className="small q1";var A=ce("a");A.href="/?quests="+B.category2+"."+B.category;ae(A,ct(Listview.funcBox.getQuestCategory(B.category)));ae(C,A)}},getVisibleText:function(A){return Listview.funcBox.getQuestCategory(A.category)},sortFunc:function(B,A,D){var C=Listview.funcBox.getQuestCategory;return strcmp(C(B.category),C(A.category))}}],getItemLink:function(A){return"/?quest="+A.id}},spell:{sort:[1,2],searchable:1,filtrable:1,columns:[{id:"name",name:LANG.name,type:"text",align:"left",span:2,value:"name",compute:function(D,B,H){var C=ce("td"),K;C.style.width="44px";C.style.padding="0";C.style.borderRight="none";if(D.creates!=null){K=g_items.createIcon(D.creates[0],1,Listview.funcBox.createTextRange(D.creates[1],D.creates[2]))}else{K=g_spells.createIcon(D.id,1)}K.style.cssFloat=K.style.styleFloat="left";ae(C,K);ae(H,C);B.style.borderLeft="none";var A=ce("div");var J=ce("a");var G=D.name.charAt(0);if(G!="@"){J.className="q"+(7-parseInt(G))}J.style.fontFamily="Verdana, sans-serif";J.href=this.template.getItemLink(D);ae(J,ct(D.name.substring(1)));ae(A,J);if(D.rank){var F=ce("div");F.className="small2";ae(F,ct(D.rank));ae(A,F)}if(D.races!=null){A.style.position="relative";var F=ce("div");F.className="small";F.style.fontStyle="italic";F.style.position="absolute";F.style.right=F.style.bottom="3px";var I=D.races.toString();if(I=="1,3,4,7,11"){ae(F,ct(g_sides[1]))}else{if(I=="2,5,6,8,10"){ae(F,ct(g_sides[2]))}else{for(var C=0,E=D.races.length;C<E;++C){if(C>0){ae(F,ct(LANG.comma))}ae(F,ct(g_chr_races[D.races[C]]))}}}ae(A,F)}ae(B,A)},getVisibleText:function(A){var B=A.name;if(A.rank){B+=" "+A.rank}if(A.races){B+=" "+Listview.funcBox.arrayText(A.races,g_chr_races)}return B}},{id:"level",name:LANG.level,width:"10%",value:"level",compute:function(A,B){if(A.level>0){return A.level}},hidden:true},{id:"school",name:LANG.school,type:"text",width:"10%",hidden:true,compute:function(A,B){return g_spell_resistances[A.school]},sortFunc:function(B,A,C){return strcmp(g_spell_resistances[B.school],g_spell_resistances[A.school])}},{id:"reagents",name:LANG.reagents,align:"left",width:"9%",getValue:function(A){return(A.reagents?A.reagents.length:0)},compute:function(F,C){var A=(F.reagents!=null);if(A){C.style.padding="0";var H=ce("div");var I=F.reagents;H.style.width=(44*I.length)+"px";for(var D=0,G=I.length;D<G;++D){var B=I[D][0];var E=I[D][1];var J=g_items.createIcon(B,1,E);J.style.cssFloat=J.style.styleFloat="left";ae(H,J)}ae(C,H)}},sortFunc:function(B,A){var D=(B.reagents!=null?B.reagents.length:0);var C=(A.reagents!=null?A.reagents.length:0);if(D>0&&D==C){return strcmp(B.reagents.toString(),A.reagents.toString())}else{return strcmp(D,C)}}},{id:"tp",name:LANG.tp,tooltip:LANG.tooltip_trainingpoints,width:"7%",hidden:true,value:"tp",compute:function(A,B){if(A.tp>0){return A.tp}}},{id:"source",name:LANG.source,type:"text",width:"12%",hidden:true,compute:function(B,E){if(B.source!=null){var D="";for(var C=0,A=B.source.length;C<A;++C){if(C>0){D+=LANG.comma}D+=g_sources[B.source[C]]}return D}},sortFunc:function(B,A,C){return Listview.funcBox.assocArrCmp(B.source,A.source,g_sources)}},{id:"skill",name:LANG.skill,type:"text",width:"16%",getValue:function(A){return A.learnedat},compute:function(E,C,H,K){if(E.skill!=null){var B=ce("div");B.className="small";for(var D=0,G=E.skill.length;D<G;++D){if(D>0){ae(B,ct(LANG.comma))}if(E.skill[D]==-1){ae(B,ct(LANG.ellipsis))}else{if(in_array([7,-2,-3,-5,-6,-7,11,9],E.cat)!=-1){var I=ce("a");I.className="q1";if(in_array([-5,-6,-7],E.cat)!=-1){I.href="/?spells="+E.cat}else{I.href="/?spells="+E.cat+"."+(E.chrclass?E.chrclass+".":"")+E.skill[D]}ae(I,ct(g_spell_skills[E.skill[D]]));ae(B,I)}else{ae(B,ct(g_spell_skills[E.skill[D]]))}}}if(E.learnedat>0){ae(B,ct(" ("));var F=ce("span");if(E.learnedat==9999){F.className="q0";ae(F,ct("??"))}else{if(E.learnedat>0){ae(F,ct(E.learnedat));F.style.fontWeight="bold"}}ae(B,F);ae(B,ct(")"))}ae(C,B);if(E.colors!=null){this.template.columns[K].type=null;var A=E.colors,J=0;for(var D=0;D<A.length;++D){if(A[D]>0){++J;break}}if(J>0){J=0;B=ce("div");B.className="small";B.style.fontWeight="bold";for(var D=0;D<A.length;++D){if(A[D]>0){if(J++>0){ae(B,ct(" "))}var L=ce("span");L.className="r"+(D+1);ae(L,ct(A[D]));ae(B,L)}}ae(C,B)}}}},getVisibleText:function(A){var B=Listview.funcBox.arrayText(A.skill,g_spell_skills);if(A.learnedat>0){B+=" "+(A.learnedat==9999?"??":A.learnedat)}return B},sortFunc:function(B,A){var D=strcmp(B.learnedat,A.learnedat);if(D!=0){return D}if(B.colors!=null&&A.colors!=null){for(var C=0;C<4;++C){D=strcmp(B.colors[C],A.colors[C]);if(D!=0){return D}}}return Listview.funcBox.assocArrCmp(B.skill,A.skill,g_spell_skills)}}],getItemLink:function(A){return"/?spell="+A.id}},zone:{sort:[1],nItemsPerPage:-1,searchable:1,filtrable:1,columns:[{id:"name",name:LANG.name,type:"text",align:"left",value:"name",compute:function(B,D){var A=ce("a");A.style.fontFamily="Verdana, sans-serif";A.href=this.template.getItemLink(B);ae(A,ct(B.name));if(B.expansion){var C=ce("span");C.className=(B.expansion==1?"bc-icon":"wotlk-icon");ae(C,A);ae(D,C)}else{ae(D,A)}},getVisibleText:function(A){var B=A.name;if(A.expansion==1){B+=" bc"}else{if(A.expansion==2){B+="wotlk wrath"}}return B}},{id:"level",name:LANG.level,type:"range",width:"10%",getMinValue:function(A){return A.minlevel},getMaxValue:function(A){return A.maxlevel},compute:function(A,B){if(A.minlevel>0&&A.maxlevel>0){if(A.minlevel!=A.maxlevel){return A.minlevel+LANG.hyphen+A.maxlevel}else{return A.minlevel}}},sortFunc:function(B,A,C){if(C>0){return strcmp(B.minlevel,A.minlevel)||strcmp(B.maxlevel,A.maxlevel)}else{return strcmp(B.maxlevel,A.maxlevel)||strcmp(B.minlevel,A.minlevel)}}},{id:"territory",name:LANG.territory,type:"text",width:"13%",compute:function(A,C){var B=ce("span");switch(A.territory){case 0:B.className="alliance-icon";break;case 1:B.className="horde-icon";break;case 4:B.className="ffapvp-icon";break}ae(B,ct(g_zone_territories[A.territory]));ae(C,B)},getVisibleText:function(A){return g_zone_territories[A.territory]},sortFunc:function(B,A,C){return strcmp(g_zone_territories[B.territory],g_zone_territories[A.territory])}},{id:"instancetype",name:LANG.instancetype,type:"text",compute:function(A,D){if(A.instance>0){var B=ce("span");if((A.instance>=1&&A.instance<=5)||A.instance==7){B.className="instance-icon"+A.instance}if(A.nplayers==-2){A.nplayers="10/25"}var C=g_zone_instancetypes[A.instance];if(A.nplayers&&((A.instance!=2&&A.instance!=5)||A.nplayers>5)){C+=" (";if(A.instance==4){C+=sprintf(LANG.lvzone_xvx,A.nplayers,A.nplayers)}else{C+=sprintf(LANG.lvzone_xman,A.nplayers)}C+=")"}ae(B,ct(C));ae(D,B)}},getVisibleText:function(A){if(A.instance>0){var B=g_zone_instancetypes[A.instance];if(A.nplayers&&((A.instance!=2&&A.instance!=5)||A.nplayers>5)){if(A.instance==4){B+=" "+sprintf(LANG.lvzone_xvx,A.nplayers,A.nplayers)}else{B+=" "+sprintf(LANG.lvzone_xman,A.nplayers)}}return B}},sortFunc:function(B,A,C){return strcmp(g_zone_instancetypes[B.instance],g_zone_instancetypes[A.instance])||strcmp(B.instance,A.instance)||strcmp(B.nplayers,A.nplayers)}},{id:"category",name:LANG.category,type:"text",width:"15%",compute:function(B,C){C.className="small q1";var A=ce("a");A.href="/?zones="+B.category;ae(A,ct(g_zone_categories[B.category]));ae(C,A)},getVisibleText:function(A){return g_zone_categories[A.category]},sortFunc:function(B,A,C){return strcmp(g_zone_categories[B.category],g_zone_categories[A.category])}}],getItemLink:function(A){return"/?zone="+A.id}},comment:{sort:[1],mode:2,nItemsPerPage:40,poundable:2,columns:[{value:"number"},{value:"id"},{value:"rating"}],compute:function(V,i){var p,T=new Date(V.date),B=(g_serverTime-T)/1000,E=(g_user.roles&26)!=0,l=V.rating<0||V.purged||V.deleted,e=E||V.user.toLowerCase()==g_user.name.toLowerCase(),G=e&&V.deleted==0,j=e&&V.replyTo!=V.id,o=((V.roles&190)==0),f=V.purged==0&&V.deleted==0&&g_user.id&&V.user.toLowerCase()!=g_user.name.toLowerCase()&&in_array(V.raters,g_user.id,function(q){return q[0]})==-1,I=V.rating>=0&&(g_user.id==0||f);V.ratable=f;i.className="comment";if(V.indent){i.className+=" comment-indent"}var Y=ce("div");var N=ce("div");var L=ce("div");V.divHeader=Y;V.divBody=N;V.divLinks=L;Y.className=(l?"comment-header-bt":"comment-header");var H=ce("div");H.className="comment-rating";if(l){var Q=ce("a");Q.href="javascript:;";Q.onclick=Listview.funcBox.coToggleVis.bind(Q,V);ae(Q,ct(LANG.lvcomment_show));ae(H,Q);ae(H,ct(" "+String.fromCharCode(160)+" "))}var P=ce("b");ae(P,ct(LANG.lvcomment_rating));var R=ce("span");ae(R,ct((V.rating>0?"+":"")+V.rating));ae(P,R);ae(H,P);ae(H,ct(" "));var C=ce("span");var J=ce("a"),n=ce("a");if(f){J.href=n.href="javascript:;";J.onclick=Listview.funcBox.coRate.bind(J,V,1);n.onclick=Listview.funcBox.coRate.bind(n,V,-1);if(E){var D=ce("a");D.href="javascript:;";D.onclick=Listview.funcBox.coRate.bind(D,V,0);D.onmouseover=Listview.funcBox.coCustomRatingOver;D.onmousemove=Tooltip.cursorUpdate;D.onmouseout=Tooltip.hide;ae(D,ct("[~]"));ae(C,D);ae(C,ct(" "))}}else{J.href=n.href="/?account=signin"}ae(J,ct("[+]"));J.onmouseover=Listview.funcBox.coPlusRatingOver;n.onmouseover=Listview.funcBox.coMinusRatingOver;J.onmousemove=n.onmousemove=Tooltip.cursorUpdate;J.onmouseout=n.onmouseout=Tooltip.hide;ae(n,ct("[-]"));ae(C,n);ae(C,ct(" "));ae(C,J);ae(H,C);if(!I){C.style.display="none"}ae(Y,H);ae(Y,ct(LANG.lvcomment_by));var h=ce("a");h.href="/?user="+V.user;ae(h,ct(V.user));ae(Y,h);ae(Y,ct(" "));var A=ce("a");A.className="q0";A.id="comments:id="+V.id;A.href="#"+A.id;Listview.funcBox.coFormatDate(A,B,T);A.style.cursor="pointer";ae(Y,A);ae(Y,ct(LANG.lvcomment_patch1+g_getPatchVersion(T)+LANG.lvcomment_patch2));ae(i,Y);N.className="comment-body"+Listview.funcBox.coGetColor(V);if(V.indent){N.className+=" comment-body-indent"}N.innerHTML=Markup.toHtml(V.body,{mode:Markup.MODE_COMMENT,roles:V.roles});ae(i,N);if((V.roles&26)==0||g_user.roles&26){var g=ce("div");V.divLastEdit=g;g.className="comment-lastedit";ae(g,ct(LANG.lvcomment_lastedit));var O=ce("a");ae(O,ct(" "));ae(g,O);ae(g,ct(" "));var F=ce("span");ae(g,F);ae(g,ct(" "));Listview.funcBox.coUpdateLastEdit(V);if(l){g.style.display="none"}ae(i,g)}L.className="comment-links";if(e){var k=ce("span");var Z=ce("a");ae(Z,ct(LANG.lvcomment_edit));Z.onclick=Listview.funcBox.coEdit.bind(this,V,0);ns(Z);Z.href="javascript:;";ae(k,Z);ae(k,ct("|"));ae(L,k)}if(G){var M=ce("span");var K=ce("a");ae(K,ct(LANG.lvcomment_delete));K.onclick=Listview.funcBox.coDelete.bind(this,V);ns(K);K.href="javascript:;";ae(M,K);ae(M,ct("|"));ae(L,M)}if(j){var a=ce("span");var X=ce("a");ae(X,ct(LANG.lvcomment_detach));X.onclick=Listview.funcBox.coDetach.bind(this,V);ns(X);X.href="javascript:;";ae(a,X);ae(a,ct("|"));ae(L,a)}if(o){var W=ce("span");var U=ce("a");ae(U,ct(LANG.lvcomment_report));if(g_user.id>0){U.onclick=Listview.funcBox.coReportClick.bind(U,V,0);U.href="javascript:;"}else{U.href="/?account=signin"}ae(W,U);ae(W,ct("|"));ae(L,W)}var S=ce("a");ae(S,ct(LANG.lvcomment_reply));if(g_user.id>0){S.onclick=Listview.funcBox.coReply.bind(this,V);S.href="javascript:;"}else{S.href="/?account=signin"}ae(L,S);if(l){N.style.display="none";L.style.display="none"}ae(i,L)},createNote:function(F){var E=ce("small");var A=ce("a");if(g_user.id>0){A.href="javascript:;";A.onclick=co_addYourComment}else{A.href="/?account=signin"}ae(A,ct(LANG.lvcomment_add));ae(E,A);var D=ce("span");D.style.padding="0 5px";D.style.color="white";ae(D,ct("|"));ae(E,D);ae(E,ct(LANG.lvcomment_sort));var C=ce("a");C.href="javascript:;";ae(C,ct(LANG.lvcomment_sortdate));C.onclick=Listview.funcBox.coSortDate.bind(this,C);ae(E,C);ae(E,ct(LANG.comma));var B=ce("a");B.href="javascript:;";ae(B,ct(LANG.lvcomment_sortrating));B.onclick=Listview.funcBox.coSortHighestRatedFirst.bind(this,B);ae(E,B);C.onclick();ae(F,E)},onNoData:function(C){if(typeof g_pageInfo=="object"&&g_pageInfo.type>0){var A="<b>"+LANG.lvnodata_co1+'</b><div class="pad2"></div>';if(g_user.id>0){var B=LANG.lvnodata_co2;B=B.replace("<a>",'<a href="javascript:;" onclick="co_addYourComment()" onmousedown="return false">');A+=B}else{var B=LANG.lvnodata_co3;B=B.replace("<a>",'<a href="/?account=signin">');B=B.replace("<a>",'<a href="/?account=signup">');A+=B}C.style.padding="1.5em 0";C.innerHTML=A}},onBeforeCreate:function(){if(location.hash.match(/:id=([0-9]+)/)!=null){var A=in_array(this.data,parseInt(RegExp.$1),function(B){return B.id});this.rowOffset=this.getRowOffset(A);return this.data[A]}},onAfterCreate:function(A){if(A!=null){var B=A.__div;this.tabs.__st=B;B.firstChild.style.border="1px solid #505050"}}},commentpreview:{sort:[3],nItemsPerPage:75,columns:[{id:"subject",name:LANG.subject,align:"left",value:"subject",compute:function(D,C){var A=ce("a");A.style.fontFamily="Verdana, sans-serif";A.href=this.template.getItemLink(D);ae(A,ct(D.subject));ae(C,A);var B=ce("div");B.className="small";ae(B,ct(LANG.types[D.type][0]));ae(C,B)}},{id:"preview",name:LANG.preview,align:"left",width:"50%",value:"preview",compute:function(H,G){var E=ce("div");E.className="crop";if(H.rating>=10){E.className+=" comment-green"}ae(E,ct(Markup.removeTags(H.preview,{mode:(H.rating!=null?Markup.MODE_COMMENT:Markup.MODE_ARTICLE)})));ae(G,E);var A=H.rating!=null;var D=H.user!=null;if(A||D){E=ce("div");E.className="small3";if(D){ae(E,ct(LANG.lvcomment_by));var B=ce("a");B.href="/?user="+H.user;ae(B,ct(H.user));ae(E,B);if(A){ae(E,ct(LANG.hyphen))}}if(A){ae(E,ct(LANG.lvcomment_rating+(H.rating>0?"+":"")+H.rating));var C=ce("span"),F="";C.className="q10";if(H.deleted){F=LANG.lvcomment_deleted}else{if(H.purged){F=LANG.lvcomment_purged}}ae(C,ct(F));ae(E,C)}ae(G,E)}}},{id:"posted",name:LANG.posted,width:"16%",value:"elapsed",compute:function(E,D){var B=new Date(E.date),A=(g_serverTime-B)/1000;var C=ce("span");Listview.funcBox.coFormatDate(C,A,B,0,1);ae(D,C)}}],getItemLink:function(A){return"/?"+g_types[A.type]+"="+A.typeId+(A.id!=null?"#comments:id="+A.id:"")}},screenshot:{sort:[],mode:3,nItemsPerPage:40,nItemsPerRow:4,poundable:2,columns:[],compute:function(I,E,H){var O,M=new Date(I.date),B=(g_serverTime-M)/1000;E.className="screenshot-cell";E.vAlign="bottom";var N=ce("a");N.href="#screenshots:id="+I.id;N.onclick=rf2;var R=ce("img"),P=Math.min(150/I.width,150/I.height);R.src="http://static.wowhead.com/uploads/screenshots/thumb/"+I.id+".jpg";ae(N,R);ae(E,N);var L=ce("div");L.className="screenshot-cell-user";var J=(I.user!=null&&I.user.length);if(J){N=ce("a");N.href="/?user="+I.user;ae(N,ct(I.user));ae(L,ct(LANG.lvscreenshot_from));ae(L,N);ae(L,ct(" "))}var G=ce("span");if(J){Listview.funcBox.coFormatDate(G,B,M)}else{Listview.funcBox.coFormatDate(G,B,M,0,1)}ae(L,G);ae(E,L);L=ce("div");L.style.position="relative";L.style.height="1em";if(g_locale.id!=0&&I.caption){I.caption=""}var A=(I.caption!=null&&I.caption.length);var F=(I.subject!=null&&I.subject.length);if(A||F){var Q=ce("div");Q.className="screenshot-caption";if(F){var D=ce("small");ae(D,ct(LANG.types[I.type][0]+LANG.colon));var C=ce("a");ae(C,ct(I.subject));C.href="/?"+g_types[I.type]+"="+I.typeId;ae(D,C);ae(Q,D);if(A&&I.caption.length){ae(D,ct(" (...)"))}ae(D,ce("br"))}if(A){aE(E,"mouseover",Listview.funcBox.ssCellOver.bind(Q));aE(E,"mouseout",Listview.funcBox.ssCellOut.bind(Q));var K=ce("span");K.innerHTML=I.caption;ae(Q,K)}ae(L,Q)}aE(E,"click",Listview.funcBox.ssCellClick.bind(this,H));ae(E,L)},createNote:function(C){if(typeof g_pageInfo=="object"&&g_pageInfo.type>0){var B=ce("small");var A=ce("a");if(g_user.id>0){A.href="javascript:;";A.onclick=ss_submitAScreenshot}else{A.href="/?account=signin"}ae(A,ct(LANG.lvscreenshot_submit));ae(B,A);ae(C,B)}},onNoData:function(C){if(typeof g_pageInfo=="object"&&g_pageInfo.type>0){var A="<b>"+LANG.lvnodata_ss1+'</b><div class="pad2"></div>';if(g_user.id>0){var B=LANG.lvnodata_ss2;B=B.replace("<a>",'<a href="javascript:;" onclick="ss_submitAScreenshot()" onmousedown="return false">');A+=B}else{var B=LANG.lvnodata_ss3;B=B.replace("<a>",'<a href="/?account=signin">');B=B.replace("<a>",'<a href="/?account=signup">');A+=B}C.style.padding="1.5em 0";C.innerHTML=A}else{return -1}},onBeforeCreate:function(){if(location.hash.match(/:id=([0-9]+)/)!=null){var A=in_array(this.data,parseInt(RegExp.$1),function(B){return B.id});this.rowOffset=this.getRowOffset(A);return A}},onAfterCreate:function(A){if(A!=null){setTimeout((function(){ScreenshotViewer.show({screenshots:this.data,pos:A})}).bind(this),1)}}},pet:{sort:[1],nItemsPerPage:-1,searchable:1,filtrable:1,columns:[{id:"name",name:LANG.name,type:"text",align:"left",value:"name",span:2,compute:function(A,H,E){var C=ce("td");C.style.width="1px";C.style.padding="0";C.style.borderRight="none";ae(C,Icon.create(A.icon,0));ae(E,C);H.style.borderLeft="none";var G=ce("div");var B=ce("a");B.style.fontFamily="Verdana, sans-serif";B.href=this.template.getItemLink(A);ae(B,ct(A.name));if(A.expansion){var D=ce("span");D.className=(A.expansion==1?"bc-icon":"wotlk-icon");ae(D,B);ae(G,D)}else{ae(G,B)}if(A.exotic){G.style.position="relative";var F=ce("div");F.className="small";F.style.fontStyle="italic";F.style.position="absolute";F.style.right="3px";F.style.bottom="0px";ae(F,ct(LANG.lvpet_exotic));ae(G,F)}ae(H,G)},getVisibleText:function(A){var B=A.name;if(A.expansion==1){B+=" bc"}else{if(A.expansion==2){B+="wotlk wrath"}}if(A.exotic){B+=" "+LANG.lvpet_exotic}return B}},{id:"level",name:LANG.level,type:"range",getMinValue:function(A){return A.minlevel},getMaxValue:function(A){return A.maxlevel},compute:function(A,B){if(A.minlevel>0&&A.maxlevel>0){if(A.minlevel!=A.maxlevel){return A.minlevel+LANG.hyphen+A.maxlevel}else{return A.minlevel}}else{return -1}},sortFunc:function(B,A,C){if(C>0){return strcmp(B.minlevel,A.minlevel)||strcmp(B.maxlevel,A.maxlevel)}else{return strcmp(B.maxlevel,A.maxlevel)||strcmp(B.minlevel,A.minlevel)}}},{id:"damage",name:LANG.damage,value:"damage",compute:function(A,B){ae(B,this.template.getStatPct(A.damage))}},{id:"armor",name:LANG.armor,value:"armor",compute:function(A,B){ae(B,this.template.getStatPct(A.armor))}},{id:"health",name:LANG.health,value:"health",compute:function(A,B){ae(B,this.template.getStatPct(A.health))}},{id:"abilities",name:LANG.abilities,type:"text",getValue:function(B){if(!B.spells){return""}if(B.spells.length>0){var C="";for(var D=0,A=B.spells.length;D<A;++D){if(B.spells[D]){C+=g_spells[B.spells[D]]["name_"+g_locale.name]}}return C}},compute:function(A,B){if(!A.spells){return""}if(A.spells.length>0){B.style.padding="0";Listview.funcBox.createCenteredIcons(A.spells,B,"",1)}},sortFunc:function(B,A){if(!B.spells||!A.spells){return 0}return strcmp(B.spellCount,A.spellCount)||strcmp(B.spells,A.spells)},hidden:true},{id:"diet",name:LANG.diet,type:"text",compute:function(A,E){if(E){E.className="small"}var B=0,C="";for(var D in g_pet_foods){if(A.diet&D){if(B++>0){C+=LANG.comma}C+=g_pet_foods[D]}}return C},sortFunc:function(B,A){return strcmp(A.foodCount,B.foodCount)||Listview.funcBox.assocArrCmp(B.diet,A.diet,g_pet_foods)}},{id:"type",name:LANG.type,type:"text",compute:function(A,C){if(A.type!=null){C.className="small q1";var B=ce("a");B.href="/?pets="+A.type;ae(B,ct(g_pet_types[A.type]));ae(C,B)}},getVisibleText:function(A){if(A.type!=null){return g_pet_types[A.type]}},sortFunc:function(B,A,C){return strcmp(g_pet_types[B.type],g_pet_types[A.type])}}],getItemLink:function(A){return"/?pet="+A.id},getStatPct:function(A){var B=ce("span");if(!isNaN(A)&&A>0){B.className="q2";ae(B,ct("+"+A+"%"))}else{if(!isNaN(A)&&A<0){B.className="q10";ae(B,ct(A+"%"))}}return B}},achievement:{sort:[1,2],nItemsPerPage:100,searchable:1,filtrable:1,columns:[{id:"name",name:LANG.name,type:"text",align:"left",value:"name",span:2,compute:function(A,F,D){var C=ce("td");C.style.width="1px";C.style.padding="0";C.style.borderRight="none";ae(C,g_achievements.createIcon(A.id,1));ae(D,C);F.style.borderLeft="none";var B=ce("a");B.style.fontFamily="Verdana, sans-serif";B.href=this.template.getItemLink(A);ae(B,ct(A.name));ae(F,B);if(A.description!=null){var E=ce("div");E.className="small";ae(E,ct(A.description));ae(F,E)}},getVisibleText:function(A){var B=A.name;if(A.description){B+=" "+A.description}return B}},{id:"location",name:LANG.location,type:"text",width:"15%",compute:function(A,C){if(A.zone){var B=ce("a");B.className="q1";B.href="/?zone="+A.zone;ae(B,ct(g_zones[A.zone]));ae(C,B)}},getVisibleText:function(A){return Listview.funcBox.arrayText(A.zone,g_zones)},sortFunc:function(B,A,C){return Listview.funcBox.assocArrCmp(B.zone,A.zone,g_zones)}},{id:"side",name:LANG.side,type:"text",width:"10%",compute:function(A,C){if(A.side){var B=ce("span");if(A.side==1){B.className="alliance-icon"}else{if(A.side==2){B.className="horde-icon"}}ae(B,ct(g_sides[A.side]));ae(C,B)}else{return -1}},getVisibleText:function(A){if(A.side){return g_sides[A.side]}},sortFunc:function(B,A,C){return strcmp(g_sides[B.side],g_sides[A.side])}},{id:"points",name:LANG.points,type:"number",width:"10%",value:"points",compute:function(A,B){if(A.points){Listview.funcBox.appendMoney(B,0,null,0,0,0,A.points)}}},{id:"category",name:LANG.category,type:"text",width:"15%",compute:function(A,C){C.className="small q1";var B=ce("a");B.href="/?achievements="+A.category;ae(B,ct(g_achievement_categories[A.category]));ae(C,B)},getVisibleText:function(A){return g_achievement_categories[A.category]},sortFunc:function(B,A,C){return strcmp(g_achievement_categories[B.category],g_achievement_categories[A.category])},hidden:true}],getItemLink:function(A){return"/?achievement="+A.id}},model:{sort:[],mode:3,nItemsPerPage:40,nItemsPerRow:4,poundable:2,columns:[],compute:function(B,H,C){H.className="screenshot-cell";H.vAlign="bottom";var G=ce("div");G.className="pet-model";var A=ce("a");A.className="pet-zoom";A.href="javascript:;";A.onclick=this.template.modelShow.bind(this.template,B.npcId,B.displayId);ae(G,A);var E=ce("div");E.id="pm"+B.displayId;ae(G,E);ae(H,G);G=ce("div");G.className="screenshot-cell-user";A=ce("a");A.href=this.template.getItemLink(B);ae(A,ct(B.skin));ae(G,A);ae(G,ct(" ("+B.count+")"));ae(H,G);G=ce("div");G.style.position="relative";G.style.height="1em";var F=ce("div");F.className="screenshot-caption";var D=ce("small");ae(D,ct(LANG.level+": "));ae(D,ct(B.minLevel+(B.minLevel==B.maxLevel?"":LANG.hyphen+(B.maxLevel==9999?"??":B.maxLevel))));ae(D,ce("br"));ae(F,D);ae(G,F);ae(H,G);setTimeout(this.template.appendFlash.bind(E,B),1)},getItemLink:function(A){return"/?npcs=1&filter="+(A.family?"fa="+A.family+";":"")+"minle=1;cr=35;crs=0;crv="+A.skin},modelShow:function(B,A){ModelViewer.show({type:1,typeId:B,displayId:A,onHide:this.modelHide.bind(this)});this.enableViewers(false)},modelHide:function(){this.enableViewers(true)},enableViewers:function(B){var D=gE(ge("lkljbjkb574"),"object");for(var C=0,A=D.length;C<A;++C){D[C].style.display=(B?"":"none")}},appendFlash:function(A){var C={model:A.displayId,modelType:8,contentPath:"http://static.wowhead.com/modelviewer/",blur:(OS.mac?"0":"1")};var B={quality:"high",allowscriptaccess:"always",menu:false,bgcolor:"#101010"};swfobject.embedSWF("http://static.wowhead.com/modelviewer/ModelView.swf",this.id,"100%","100%","10.0.0","http://static.wowhead.com/modelviewer/expressInstall.swf",C,B)}}};Menu.fixUrls(mn_items,"/?items=");Menu.fixUrls(mn_itemSets,"/?itemsets&filter=cl=","#0-2+1");Menu.fixUrls(mn_npcs,"/?npcs=");Menu.fixUrls(mn_objects,"/?objects=");Menu.fixUrls(mn_quests,"/?quests=");Menu.fixUrls(mn_spells,"/?spells=");Menu.fixUrls(mn_zones,"/?zones=");Menu.fixUrls(mn_pets,"/?pets=");Menu.fixUrls(mn_factions,"/?factions=");Menu.fixUrls(mn_achievements,"/?achievements=",null,true);Menu.fixUrls(mn_talentCalc,"/?talent=");Menu.fixUrls(mn_petCalc,"/?petcalc=");Menu.fixUrls(mn_forums,"/?forums&board=",null,true);var g_locale={id:0,name:"enus"};var g_localTime=new Date();var g_user={id:0,name:"",roles:0};var g_items={};var g_quests={};var g_spells={};var g_achievements={};var g_users={};var g_types={1:"npc",2:"object",3:"item",4:"itemset",5:"quest",6:"spell",7:"zone",8:"faction",9:"pet",10:"achievement"};var g_locales={0:"enus",2:"frfr",3:"dede",6:"eses",7:"ruru",25:"ptr"};var g_customColors={Miyari:"pink"};g_items.add=function(B,A){if(g_items[B]!=null){cO(g_items[B],A)}else{g_items[B]=A}};g_items.getIcon=function(A){if(g_items[A]!=null){return g_items[A].icon}else{return"inv_misc_questionmark"}};g_items.createIcon=function(D,B,A,C){return Icon.create(g_items.getIcon(D),B,null,"./?i="+D,A,C)};g_spells.getIcon=function(A){if(g_spells[A]!=null){return g_spells[A].icon}else{return"inv_misc_questionmark"}};g_spells.createIcon=function(D,B,A,C){return Icon.create(g_spells.getIcon(D),B,null,"/?spell="+D,A,C)};g_achievements.getIcon=function(A){if(g_achievements[A]!=null){return g_achievements[A].icon}else{return"inv_misc_questionmark"}};g_achievements.createIcon=function(D,B,A,C){return Icon.create(g_achievements.getIcon(D),B,null,"/?achievement="+D)};var $WowheadPower=new function(){var D,Y,g,T,N,h,P,T,j,X,V,E=0,a=0,G=1,F=2,U=3,e=4,O=3,S=5,L=6,Z=10,I=15,R=15,K={3:[g_items,"item"],5:[g_quests,"quest"],6:[g_spells,"spell"],10:[g_achievements,"achievement"]};function k(){aE(document,"mouseover",M)}function J(o){var p=g_getCursorPos(o);X=p.x;V=p.y}function l(AA,x){if(AA.nodeName!="A"&&AA.nodeName!="AREA"){return -2323}if(!AA.href.length){return }if(AA.rel.indexOf("np")!=-1){return }var w,u,r,q,s={};var o=function(p,AB,t){if(AB=="buff"){s[AB]=true}else{if(AB=="rand"||AB=="ench"||AB=="lvl"){s[AB]=parseInt(t)}else{if(AB=="gems"||AB=="pcs"){s[AB]=t.split(":")}}}};u=2;r=3;if(AA.href.indexOf("http://")==0){w=1;q=AA.href.match(/http:\/\/(www|dev|fr|es|de|ru|wotlk|ptr)?\.?wowhead\.com\/\?(item|quest|spell|achievement)=([0-9]+)/)}else{q=AA.href.match(/()\?(item|quest|spell|achievement)=([0-9]+)/)}if(q==null&&AA.rel){w=0;u=1;r=2;q=AA.rel.match(/(item|quest|spell|achievement).?([0-9]+)/)}AA.href.replace(/([a-zA-Z]+).?([0-9:\\-]*)/g,o);if(AA.rel){AA.rel.replace(/([a-zA-Z]+).?([0-9:\\-]*)/g,o)}if(q){var z,v="www";if(w&&q[w]){v=q[w]}else{var y=location.hostname.match(/(www|dev|fr|es|de|ru|wotlk|ptr)\.?wowhead\.com/);if(y!=null){v=y[1]}}z=g_getLocaleFromDomain(v);if(v=="wotlk"){v="www"}T=v;j=AA;if(AA.href.indexOf("#")!=-1&&document.location.href.indexOf(q[u]+"="+q[r])!=-1){return }E=(AA.parentNode.className=="tile"?1:0);if(!AA.onmouseout){if(E==0){AA.onmousemove=A}AA.onmouseout=B}J(x);Q(g_getIdFromTypeName(q[u]),q[r],z,s)}}function M(q){q=$E(q);var p=q._target;var o=0;while(p!=null&&o<3&&l(p,q)==-2323){p=p.parentNode;++o}}function A(o){o=$E(o);J(o);Tooltip.move(X,V,0,0,I,R)}function B(){D=null;j=null;Tooltip.hide()}function i(o){return(P?"buff_":"tooltip_")+g_locales[o]}function f(q,r,p){var o=K[q][0];if(o[r]==null){o[r]={}}if(o[r].status==null){o[r].status={}}if(o[r].status[p]==null){o[r].status[p]=a}}function Q(r,t,p,s){if(!s){s={}}var q=t+(s.rand?"r"+s.rand:"")+(s.ench?"e"+s.ench:"")+(s.gems?"g"+s.gems.join(","):"")+(s.sock?"s":"");D=r;Y=q;g=p;N=s.pcs;h=s.lvl;P=s.buff;f(r,q,p);var o=K[r][0];if(o[q].status[p]==e||o[q].status[p]==U){n(o[q][i(p)],o[q].icon,N,h)}else{if(o[q].status[p]==G){n(LANG.tooltip_loading)}else{C(r,t,p,null,s)}}}function C(v,o,x,u,r){var w=o+(r.rand?"r"+r.rand:"")+(r.ench?"e"+r.ench:"")+(r.gems?"g"+r.gems.join(","):"")+(r.sock?"s":"");var t=K[v][0];if(t[w].status[x]!=a&&t[w].status[x]!=F){return }t[w].status[x]=G;if(!u){t[w].timer=setTimeout(function(){H.apply(this,[v,w,x])},333)}var q="";for(var s in r){if(s!="rand"&&s!="ench"&&s!="gems"&&s!="sock"){continue}if(typeof r[s]=="object"){q+="&"+s+"="+r[s].join(":")}else{if(s=="sock"){q+="&sock"}else{q+="&"+s+"="+r[s]}}}g_ajaxIshRequest("http://"+T+".wowhead.com/?"+K[v][1]+"="+o+q+"&locale="+x+"&power&lol")}function n(r,s,t,p){if(j._fixTooltip){r=j._fixTooltip(r,D,Y,j)}if(!r){r=LANG["tooltip_"+g_types[D]+"notfound"];s="inv_misc_questionmark"}else{if(t&&t.length){var u=0;for(var q=0,o=t.length;q<o;++q){if(m=r.match(new RegExp("<span><!--si([0-9]+:)*"+t[q]+"(:[0-9]+)*-->"))){r=r.replace(m[0],'<span class="q8"><!--si'+t[q]+"-->");++u}}if(u>0){r=r.replace("(0/","("+u+"/");r=r.replace(new RegExp("<span>\\(([0-"+u+"])\\)","g"),'<span class="q2">($1)')}}if(p){r=r.replace(/\(<!--r([0-9]+):([0-9]+):([0-9]+)-->([0-9.%]+)(.+?)([0-9]+)\)/g,function(x,x,y,w,x,AA,x){var z=convertRatingToPercent(p,y,w);z=(Math.round(z*100)/100);if(y!=12&&y!=37){z+="%"}return"(<!--r"+p+":"+y+":"+w+"-->"+z+AA+p+")"})}}if(E==1){Tooltip.setIcon(null);Tooltip.show(j,r,0,0)}else{Tooltip.setIcon(s);Tooltip.showAtXY(r,X,V,I,R)}}function H(q,r,p){if(D==q&&Y==r&&g==p){n(LANG.tooltip_loading);var o=K[q][0];o[r].timer=setTimeout(function(){W.apply(this,[q,r,p])},3850)}}function W(q,r,p){var o=K[q][0];o[r].status[p]=F;if(D==q&&Y==r&&g==p){n(LANG.tooltip_noresponse)}}this.register=function(r,s,p,q){var o=K[r][0];if(o[s]==null){f(r,s,p)}else{clearTimeout(o[s].timer)}cO(o[s],q);if(o[s].status[p]==G){if(o[s][i(p)]){o[s].status[p]=e}else{o[s].status[p]=U}}if(D==r&&s==Y&&g==p){n(o[s][i(p)],o[s].icon,N,h)}};this.registerItem=function(q,o,p){this.register(O,q,o,p)};this.registerQuest=function(q,o,p){this.register(S,q,o,p)};this.registerSpell=function(q,o,p){this.register(L,q,o,p)};this.registerAchievement=function(q,o,p){this.register(Z,q,o,p)};this.request=function(q,t,o,r){if(!r){r={}}var p=t+(r.rand?"r"+r.rand:"")+(r.ench?"e"+r.ench:"")+(r.gems?"g"+r.gems.join(","):"")+(r.sock?"s":"");f(q,p,o);var s=location.hostname.match(/(www|dev|fr|es|de|wotlk)\.?wowhead\.com/);if(s!=null){domain=s[1]}else{domain="www"}T=domain;C(q,t,o,1,r)};this.requestItem=function(p,o){this.request(O,p,g_locale.id,o)};this.requestSpell=function(o){this.request(L,o,g_locale.id)};this.getStatus=function(q,r,p){var o=K[q][0];if(o[r]!=null){return o[r].status[p]}else{return a}};this.getItemStatus=function(p,o){this.getStatus(O,p,o)};this.getSpellStatus=function(p,o){this.getStatus(L,p,o)};k()};var LiveSearch=new function(){var currentTextbox,lastSearch={},lastDiv,timer,prepared,container,cancelNext,hasData,summary,selection;function setText(textbox,txt){textbox.value=txt;textbox.selectionStart=textbox.selectionEnd=txt.length}function colorDiv(div,fromOver){if(lastDiv){lastDiv.className=lastDiv.className.replace("live-search-selected","")}lastDiv=div;lastDiv.className+=" live-search-selected";selection=div.i;if(!fromOver){show();setTimeout(setText.bind(0,currentTextbox,g_getTextContent(div.firstChild.firstChild.childNodes[1])),1);cancelNext=1}}function aOver(){colorDiv(this.parentNode.parentNode,1)}function isVisible(){if(!container){return false}return container.style.display!="none"}function adjust(fromResize){if(fromResize==1&&!isVisible()){return }if(currentTextbox==null){return }var c=ac(currentTextbox);container.style.left=(c[0]-2)+"px";container.style.top=(c[1]+currentTextbox.offsetHeight+1)+"px";container.style.width=currentTextbox.offsetWidth+"px"}function prepare(){if(prepared){return }prepared=1;container=ce("div");container.className="live-search";container.style.display="none";ae(ge("layers"),container);aE(window,"resize",adjust.bind(0,1));aE(document,"click",hide)}function show(){if(container&&!isVisible()){adjust();container.style.display=""}}function hide(){if(container){container.style.display="none"}}function boldify(match){return"<b>"+match+"</b>"}function display(textbox,search,suggz,dataz){prepare();show();lastA=null;hasData=1;selection=null;while(container.firstChild){de(container.firstChild)}if(!Browser.ie6){ae(container,ce("em"));ae(container,ce("var"));ae(container,ce("strong"))}search=search.replace(/[^a-z0-9\-]/i," ");search=trim(search.replace(/\s+/g," "));var regex=g_createOrRegex(search);for(var i=0,len=suggz.length;i<len;++i){var pos=suggz[i].lastIndexOf("(");if(pos!=-1){suggz[i]=suggz[i].substr(0,pos-1)}var type=dataz[i][0],typeId=dataz[i][1],param1=dataz[i][2],param2=dataz[i][3],a=ce("a"),sp=ce("i"),sp2=ce("span"),div=ce("div"),div2=ce("div");div.i=i;a.onmouseover=aOver;if(textbox._summary){a.href="javascript:;";a.onmousedown=Summary.addGroupItem.bind(g_summaries[textbox._summary],textbox._type,typeId);a.onclick=rf;a.rel=g_types[type]+"="+typeId}else{a.href="/?"+g_types[type]+"="+typeId}if(type==3&&param2!=null){a.className+=" q"+param2}if((type==3||type==6||type==9||type==10)&&param1){div.className+=" live-search-icon";div.style.backgroundImage="url(http://static.wowhead.com/images/icons/small/"+param1.toLowerCase()+".jpg)"}else{if(type==5&&param1>=1&&param1<=2){div.className+=" live-search-icon-quest-"+(param1==1?"alliance":"horde")}}if(!textbox._summary){ae(sp,ct(LANG.types[type][0]))}ae(a,sp);var buffer=suggz[i];buffer=buffer.replace(regex,boldify);sp2.innerHTML=buffer;ae(a,sp2);if(type==6&&param2){ae(a,ct(" ("+param2+")"))}ae(div2,a);ae(div,div2);ae(container,div)}}function receive(xhr,opt){var text=xhr.responseText;if(text.charAt(0)!="["||text.charAt(text.length-1)!="]"){return }var a=eval(text);var search=a[0];if(search==opt.search){if(opt.textbox._summary&&a.length==9){for(var i=0,len=a[8].length;i<len;++i){if(in_array(g_summaries[opt.textbox._summary].data,a[8][i].id,function(x){return x.id})==-1){g_summaries[opt.textbox._summary].data.push(a[8][i]);g_items.add(a[8][i].id,{icon:a[8][i].icon})}}a.splice(8,1)}if(a.length==8){display(opt.textbox,search,a[1],a[7])}else{hide()}}}function fetch(textbox,search){var url="/?search="+urlencode(search)+"&opensearch";if(textbox._type){url+="&type="+textbox._type}new Ajax(url,{onSuccess:receive,textbox:textbox,search:search})}function preFetch(textbox,search){if(cancelNext){cancelNext=0;return }hasData=0;if(timer>0){clearTimeout(timer);timer=0}timer=setTimeout(fetch.bind(0,textbox,search),333)}function cycle(dir){if(!isVisible()){if(hasData){show()}return }var firstNode=(container.childNodes[0].nodeName=="EM"?container.childNodes[3]:container.firstChild);var bakDiv=dir?firstNode:container.lastChild;if(lastDiv==null){colorDiv(bakDiv)}else{var div=dir?lastDiv.nextSibling:lastDiv.previousSibling;if(div){if(div.nodeName=="STRONG"){div=container.lastChild}colorDiv(div)}else{colorDiv(bakDiv)}}}function onKeyUp(e){e=$E(e);var textbox=e._target;switch(e.keyCode){case 48:case 96:case 107:case 109:if(Browser.gecko&&e.ctrlKey){adjust(textbox);break}break;case 13:if(textbox._summary&&hasData&&isVisible()){var div=container.childNodes[3+(selection|0)];div.firstChild.firstChild.onmousedown();hide();g_summaries.compare.toggleOptions()}break}var search=trim(textbox.value.replace(/\s+/g," "));if(search==lastSearch[textbox.id]){return }lastSearch[textbox.id]=search;if(search.length){preFetch(textbox,search)}else{hide()}}function onKeyDown(e){e=$E(e);var textbox=e._target;switch(e.keyCode){case 27:hide();break;case 38:cycle(0);break;case 40:cycle(1);break}}function onFocus(e){e=$E(e);var textbox=e._target;if(textbox!=document){currentTextbox=textbox}}this.attach=function(textbox){textbox.setAttribute("autocomplete","off");aE(textbox,"focus",onFocus);aE(textbox,"keyup",onKeyUp);aE(textbox,Browser.opera?"keypress":"keydown",onKeyDown)};this.reset=function(textbox){lastSearch[textbox.id]=null;hasData=0;hide()}};var Lightbox=new function(){var F,N,Q,J={},C={},P,D,E;function O(){aE(F,"click",G);aE(N,"mousewheel",B);aE(document,Browser.opera?"keypress":"keydown",H);aE(window,"resize",A);if(Browser.ie6){aE(window,"scroll",M)}}function K(){dE(F,"click",G);dE(N,"mousewheel",B);dE(document,Browser.opera?"keypress":"keydown",H);dE(window,"resize",A);if(Browser.ie6){dE(window,"scroll",M)}}function I(){if(P){return }P=1;var R=ge("layers");F=ce("div");F.className="lightbox-overlay";N=ce("div");N.className="lightbox-outer";Q=ce("div");Q.className="lightbox-inner";F.style.display=N.style.display="none";ae(R,F);ae(N,Q);ae(R,N)}function B(R){R=$E(R);R.returnValue=false}function H(R){R=$E(R);switch(R.keyCode){case 27:G();break}}function A(R){if(R!=1234){if(C.onResize){C.onResize()}}F.style.height=document.body.offsetHeight+"px";if(Browser.ie6){M()}}function M(){var S=g_getScroll().y,R=g_getWindowSize().h;N.style.top=(S+R/2)+"px"}function G(){K();if(C.onHide){C.onHide()}E=0;F.style.display=N.style.display="none";array_apply(gE(document,"iframe"),function(R){R.style.display=""});g_enableScroll(true)}function L(){F.style.display=N.style.display=J[D].style.display=""}this.setSize=function(R,S){Q.style.visibility="hidden";Q.style.width=R+"px";Q.style.height=S+"px";Q.style.left=-parseInt(R/2)+"px";Q.style.top=-parseInt(S/2)+"px";Q.style.visibility="visible"};this.show=function(V,S,R){E=1;C=S||{};array_apply(gE(document,"iframe"),function(W){W.style.display="none"});I();O();if(D!=V&&J[D]!=null){J[D].style.display="none"}D=V;var U=0,T;if(J[V]==null){U=1;T=ce("div");ae(Q,T);J[V]=T}else{T=J[V]}if(C.onShow){C.onShow(T,U,R)}A(1234);L()};this.reveal=function(){L()};this.hide=function(){G()};this.isVisible=function(){return E}};var ModelViewer=new function(){var J,V,X=[],L,S,D,M,C,B,O,E,R,P,Q=[{id:10,name:g_chr_races[10],model:"bloodelf"},{id:11,name:g_chr_races[11],model:"draenei"},{id:3,name:g_chr_races[3],model:"dwarf"},{id:7,name:g_chr_races[7],model:"gnome"},{id:1,name:g_chr_races[1],model:"human"},{id:4,name:g_chr_races[4],model:"nightelf"},{id:2,name:g_chr_races[2],model:"orc"},{id:6,name:g_chr_races[6],model:"tauren"},{id:8,name:g_chr_races[8],model:"troll"},{id:5,name:g_chr_races[5],model:"scourge"}],N=[{id:1,name:LANG.female,model:"female"},{id:0,name:LANG.male,model:"male"}];function U(){S.style.display="none";D.style.display="none"}function F(){var Y,Z;if(C.style.display==""){Y=(C.selectedIndex>=0?C.options[C.selectedIndex].value:"")}else{Y=(B.selectedIndex>=0?B.options[B.selectedIndex].value:"")}Z=(O.selectedIndex>=0?O.options[O.selectedIndex].value:0);return{r:Y,s:Z}}function H(Y,Z){return(!isNaN(Y)&&Y>0&&in_array(Q,Y,function(a){return a.id})!=-1&&!isNaN(Z)&&Z>=0&&Z<=1)}function W(){if(R){var e='<applet id="3dviewer-java" code="org.jdesktop.applet.util.JNLPAppletLauncher" width="600" height="400" archive="http://static.wowhead.com/modelviewer/applet-launcher.jar,http://download.java.net/media/jogl/builds/archive/jsr-231-webstart-current/jogl.jar,http://download.java.net/media/gluegen/webstart/gluegen-rt.jar,http://download.java.net/media/java3d/webstart/release/vecmath/latest/vecmath.jar,http://static.wowhead.com/modelviewer/ModelView510.jar"><param name="codebase_lookup" value="false"><param name="cache_option" value="no"><param name="subapplet.classname" value="modelview.ModelViewerApplet"><param name="subapplet.displayname" value="Model Viewer Applet"><param name="progressbar" value="true"><param name="jnlpNumExtensions" value="1"><param name="jnlpExtension1" value="http://download.java.net/media/jogl/builds/archive/jsr-231-webstart-current/jogl.jnlp"><param name="contentPath" value="http://static.wowhead.com/modelviewer/"><param name="model" value="'+J+'"><param name="modelType" value="'+V+'">';if(V==16&&X.length){e+='<param name="equipList" value="'+X.join(",")+'">'}e+='<param name="bgColor" value="#181818"></applet>';D.innerHTML=e;D.style.display=""}else{var h={model:J,modelType:V,contentPath:"http://static.wowhead.com/modelviewer/",blur:(OS.mac?"0":"1")};var g={quality:"high",allowscriptaccess:"always",menu:false,bgcolor:"#181818"};var Y={};if(V==16&&X.length){h.equipList=X.join(",")}swfobject.embedSWF("http://static.wowhead.com/modelviewer/ModelView.swf","dsjkgbdsg2346","600","400","10.0.0","http://static.wowhead.com/modelviewer/expressInstall.swf",h,g,Y);S.style.display=""}var i=F(),a=i.r,f=i.s;if(!L.noPound){var Z="#modelviewer";if(a&&f){Z+=":"+a+"+"+f}else{Z+=":"}if(L.extraPound!=null){Z+=":"+L.extraPound}location.replace(rtrim(Z,":"))}}function G(){var f=F(),Z=f.r,a=f.s;if(!Z){if(O.style.display=="none"){return }O.style.display="none";J=X[1];switch(L.slot){case 1:V=2;break;case 3:V=4;break;default:V=1}}else{if(O.style.display=="none"){O.style.display=""}if(B.style.display==""){sc("modelviewer_model_race",7,Z,"/",".wowhead.com");sc("modelviewer_model_sex",7,a,"/",".wowhead.com")}var f=function(g){return g.id};var e=in_array(Q,Z,f);var Y=in_array(N,a,f);if(e!=-1&&Y!=-1){J=Q[e].model+N[Y].model;V=16}}U();W()}function K(Y){g_setSelectedLink(this,"modelviewer-mode");U();if(R==null){R=Y;setTimeout(W,50)}else{R=Y;sc("modelviewer_mode",7,Y,"/",".wowhead.com");W()}}function A(i,Z){var j=-1,k=-1,a,f;if(Z.race!=null&&Z.sex!=null){j=Z.race;k=Z.sex;M.style.display="none";i=0}else{M.style.display=""}if(j==-1&&k==-1){if(location.hash){var h=location.hash.match(/modelviewer:([0-9]+)\+([0-9]+)/);if(h!=null){if(H(h[1],h[2])){j=h[1];k=h[2];O.style.display=""}}}}if(i){a=C;f=1;C.style.display="";C.selectedIndex=-1;B.style.display="none";if(k==-1){O.style.display="none"}}else{if(j==-1&&k==-1){var n=parseInt(gc("modelviewer_model_race")),g=parseInt(gc("modelviewer_model_sex"));if(H(n,g)){j=n;k=g}else{j=10;k=1}}a=B;f=0;C.style.display="none";B.style.display="";O.style.display=""}if(k!=-1){O.selectedIndex=k}if(j!=-1&&k!=-1){var l=function(o){return o.id};var Y=in_array(Q,j,l);var e=in_array(N,k,l);if(Y!=-1&&e!=-1){J=Q[Y].model+N[e].model;V=16;Y+=f;if(Browser.opera){setTimeout(function(){a.selectedIndex=Y},1)}else{a.selectedIndex=Y}O.selectedIndex=e}}}function I(){if(!L.noPound){if(E&&E.indexOf("modelviewer")==-1){location.replace(E)}else{location.replace("#.")}}if(L.onHide){L.onHide()}}function T(n,h,a){var e,Y;Lightbox.setSize(620,452);if(h){n.className="modelviewer";var p=ce("div");S=ce("div");D=ce("div");var l=ce("div");l.id="dsjkgbdsg2346";ae(S,l);p.className="modelviewer-screen";S.style.display=D.style.display="none";ae(p,S);ae(p,D);ae(n,p);e=ce("a"),Y=ce("a");e.className="modelviewer-help";e.href="/?help=modelviewer";e.target="_blank";ae(e,ce("span"));Y.className="modelviewer-close";Y.href="javascript:;";Y.onclick=Lightbox.hide;ae(Y,ce("span"));ae(n,Y);ae(n,e);var k=ce("div"),Z=ce("span"),e=ce("a"),Y=ce("a");k.className="modelviewer-quality";e.href=Y.href="javascript:;";ae(e,ct("Flash"));ae(Y,ct("Java"));e.onclick=K.bind(e,0);Y.onclick=K.bind(Y,1);ae(Z,e);ae(Z,ct(" "+String.fromCharCode(160)));ae(Z,Y);ae(k,ce("div"));ae(k,Z);ae(n,k);M=ce("div");M.className="modelviewer-model";var l=function(o,i){return strcmp(o.name,i.name)};Q.sort(l);N.sort(l);C=ce("select");B=ce("select");O=ce("select");C.onchange=B.onchange=O.onchange=G;ae(C,ce("option"));for(var g=0,j=Q.length;g<j;++g){var f=ce("option");f.value=Q[g].id;ae(f,ct(Q[g].name));ae(C,f)}for(var g=0,j=Q.length;g<j;++g){var f=ce("option");f.value=Q[g].id;ae(f,ct(Q[g].name));ae(B,f)}for(var g=0,j=N.length;g<j;++g){var f=ce("option");f.value=N[g].id;ae(f,ct(N[g].name));ae(O,f)}O.style.display="none";ae(M,ce("div"));ae(M,C);ae(M,B);ae(M,O);ae(n,M);k=ce("div");k.className="clear";ae(n,k)}switch(a.type){case 1:M.style.display="none";if(a.humanoid){V=32}else{V=8}J=a.displayId;break;case 2:M.style.display="none";V=64;J=a.displayId;break;case 3:X=[a.slot,a.displayId];if(in_array([4,5,6,7,8,9,10,16,19,20],a.slot)!=-1){A(0,a)}else{switch(a.slot){case 1:V=2;break;case 3:V=4;break;default:V=1}J=a.displayId;A(1,a)}break;case 4:X=a.equipList;A(0,a)}if(h){if(gc("modelviewer_mode")=="1"){Y.onclick()}else{e.onclick()}}else{U();setTimeout(W,1)}E=location.hash}this.checkPound=function(){if(location.hash&&location.hash.indexOf("#modelviewer")==0){if(P!=null){var Y=location.hash.split(":");if(Y.length==3&&Y[2]){P(Y[2])}}else{var Z=ge("dsgndslgn464d");if(Z){Z.onclick()}}}};this.addExtraPound=function(Y){P=Y};this.show=function(Y){L=Y;Lightbox.show("modelviewer",{onShow:T,onHide:I},Y)};DomContentLoaded.addEvent(this.checkPound)};var g_screenshots={};var ScreenshotViewer=new function(){var T,L,O,X,e,C,D,Q=0,S,P,E,K,V,W,Y,M,U,J;function N(g){var h=T[L];var f=Math.max(50,Math.min(618,g_getWindowSize().h-72-g));if(Q!=1||h.id||h.resize){C=Math.min(772/h.width,618/h.height);e=Math.min(772/h.width,f/h.height)}else{C=e=1}if(C>1){C=1}if(e>1){e=1}O=Math.round(e*h.width);X=Math.round(e*h.height);var i=Math.max(480,O);Lightbox.setSize(i+20,X+52+g);if(Browser.ie6){E.style.width=i+"px";if(T.length>1){V.style.height=W.style.height=X+"px"}else{Y.style.height=X+"px"}}if(g){K.firstChild.width=O;K.firstChild.height=X}}function G(h){var f=T[h],g="#screenshots:";if(Q==0){g+="id="+f.id}else{g+=S+":"+(h+1)}return g}function a(o){if(o&&(e==C)&&g_getWindowSize().h>P.offsetHeight){return }P.style.visibility="hidden";var p=T[L],i=(p.width>772||p.height>618);N(0);var f=(p.url?p.url:"http://static.wowhead.com/uploads/screenshots/"+(i?"resized/":"normal/")+p.id+".jpg");var h='<img src="'+f+'" width="'+O+'" height="'+X+'"';if(Browser.ie6){h+=' galleryimg="no"'}h+=">";K.innerHTML=h;if(!o){if(p.url){M.href=f}else{M.href="http://static.wowhead.com/uploads/screenshots/normal/"+p.id+".jpg"}if(!p.user&&typeof g_pageInfo=="object"){p.user=g_pageInfo.username}var l=(p.date&&p.user),k=(T.length>1);if(l){var j=new Date(p.date),r=(g_serverTime-j)/1000;var q=U.firstChild.childNodes[1];q.href="/?user="+p.user;q.innerHTML=p.user;var u=U.firstChild.childNodes[3];while(u.firstChild){de(u.firstChild)}Listview.funcBox.coFormatDate(u,r,j);U.firstChild.style.display=""}else{U.firstChild.style.display="none"}var u=U.childNodes[1];if(k){var t="";if(l){t+=LANG.dash}t+=(L+1)+LANG.lvpage_of+T.length;u.innerHTML=t;u.style.display=""}else{u.style.display="none"}U.style.display=(l||k?"":"none");if(g_locale.id!=0&&p.caption){p.caption=""}var n=(p.caption!=null&&p.caption.length);var g=(p.subject!=null&&p.subject.length&&p.type&&p.typeId);if(n||g){var h="";if(g){h+=LANG.types[p.type][0]+LANG.colon;h+='<a href="/?'+g_types[p.type]+"="+p.typeId+'">';h+=p.subject;h+="</a>"}if(n){if(g){h+=LANG.dash}h+=p.caption}J.innerHTML=h;J.style.display=""}else{J.style.display="none"}if(T.length>1){V.href=G(Z(-1));W.href=G(Z(1));V.style.display=W.style.display="";Y.style.display="none"}else{V.style.display=W.style.display="none";Y.style.display=""}location.replace(G(L))}Lightbox.reveal();if(J.offsetHeight>18){N(J.offsetHeight-18)}P.style.visibility="visible"}function Z(g){var f=L;f+=g;if(f<0){f=T.length-1}else{if(f>=T.length){f=0}}return f}function B(){L=Z(-1);a();return false}function A(){L=Z(1);a();return false}function F(f){f=$E(f);switch(f.keyCode){case 37:B();break;case 39:A();break}}function I(){a(1)}function H(){if(T.length>1){dE(document,"keyup",F)}if(D&&Q==0){if(D.indexOf(":id=")!=-1){D="#screenshots"}location.replace(D)}else{location.replace("#.")}}function R(f,k,g){if(typeof g.screenshots=="string"){T=g_screenshots[g.screenshots];Q=1;S=g.screenshots}else{T=g.screenshots;Q=0;S=null}P=f;L=0;if(g.pos&&g.pos>=0&&g.pos<T.length){L=g.pos}if(k){f.className="screenshotviewer";E=ce("div");E.className="screenshotviewer-screen";V=ce("a");W=ce("a");V.className="screenshotviewer-prev";W.className="screenshotviewer-next";V.href="javascript:;";W.href="javascript:;";var l=ce("span");ae(l,ce("b"));ae(V,l);var l=ce("span");ae(l,ce("b"));ae(W,l);V.onclick=B;W.onclick=A;Y=ce("a");Y.className="screenshotviewer-cover";Y.href="javascript:;";Y.onclick=Lightbox.hide;var l=ce("span");ae(l,ce("b"));ae(Y,l);if(Browser.ie6){ns(V);ns(W);V.onmouseover=W.onmouseover=Y.onmouseover=function(){this.firstChild.style.display="block"};V.onmouseout=W.onmouseout=Y.onmouseout=function(){this.firstChild.style.display=""}}ae(E,V);ae(E,W);ae(E,Y);K=ce("div");ae(E,K);ae(f,E);var i=ce("a");i.className="screenshotviewer-close";i.href="javascript:;";i.onclick=Lightbox.hide;ae(i,ce("span"));ae(f,i);M=ce("a");M.className="screenshotviewer-original";M.href="javascript:;";M.target="_blank";ae(M,ce("span"));ae(f,M);U=ce("div");U.className="screenshotviewer-from";var h=ce("span");ae(h,ct(LANG.lvscreenshot_from));ae(h,ce("a"));ae(h,ct(" "));ae(h,ce("span"));ae(U,h);ae(U,ce("span"));ae(f,U);J=ce("div");J.className="screenshotviewer-caption";ae(f,J);var j=ce("div");j.className="clear";ae(f,j)}D=location.hash;if(T.length>1){aE(document,"keyup",F)}a()}this.checkPound=function(){if(location.hash&&location.hash.indexOf("#screenshots")==0){if(!g_listviews.screenshots){var g=location.hash.split(":");if(g.length==3){var h=g_screenshots[g[1]],f=parseInt(g[2]);if(h&&f>=1&&f<=h.length){ScreenshotViewer.show({screenshots:g[1],pos:f-1})}}}}};this.show=function(f){Lightbox.show("screenshotviewer",{onShow:R,onHide:H,onResize:I},f)};DomContentLoaded.addEvent(this.checkPound)};DomContentLoaded.addEvent(function(){var H={leaderboard:[728,90],skyscraper:[160,600],medrect:[300,250]};var G={leaderboard:["header-ad"],skyscraper:["sidebar-ad"],medrect:["infobox-ad","blog-sidebar-medrect","pl-rightbar-ad","contribute-ad"]};for(var F in H){var D=G[F];for(var C=0,A=D.length;C<A;++C){var B=ge(D[C]);if(B){var E=ce("iframe");E.width=H[F][0];E.height=H[F][1];E.scrolling='no';E.frameBorder=0;E.src="http://static.wowhead.com/ads/dynamic/"+F+".html";ae(B,E);break}}}});

