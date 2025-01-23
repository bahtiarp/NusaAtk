// JavaScript Document
		/*
		
		This is one of the free scripts from www.dhtmlgoodies.com
		You are free to use this script as long as this copyright message is kept intact
		
		(c) Alf Magne Kalleland, http://www.dhtmlgoodies.com - 2005
		
		*/		
		var plusNode = 'modules/gktools/tree/images/dhtmlgoodies_plus.gif';
		var minusNode = 'modules/gktools/tree/images/dhtmlgoodies_minus.gif';
		
		var nameOfCookie = 'dhtmlgoodies_expanded';
		var initExpandedNodes = "";
		/*
          <?
		$cookieValue = "";
		if(isset($_COOKIE[$this->nameOfCookie]))$cookieValue = $_COOKIE[$this->nameOfCookie];		
		echo "var initExpandedNodes =\"".$cookieValue."\";\n";
		?>
          */
          		
		/*
		These cookie functions are downloaded from 
		http://www.mach5.com/support/analyzer/manual/html/General/CookiesJavaScript.htm
		*/
		function Get_Cookie(name) { 
		   var start = document.cookie.indexOf(name+"="); 
		   var len = start+name.length+1; 
		   if ((!start) && (name != document.cookie.substring(0,name.length))) return null; 
		   if (start == -1) return null; 
		   var end = document.cookie.indexOf(";",len); 
		   if (end == -1) end = document.cookie.length; 
		   return unescape(document.cookie.substring(len,end)); 
		} 
		
		//var cookieValue = Get_Cookie(nameOfCookie);
          //if(cookieValue <> ""){
            //   var initExpandedNodes = $cookieValue;
         // }
          
		
		// This function has been slightly modified
		function Set_Cookie(name,value,expires,path,domain,secure) { 
			expires = expires * 60*60*24*1000;
			var today = new Date();
			var expires_date = new Date( today.getTime() + (expires) );
		    var cookieString = name + "=" +escape(value) + 
		       ( (expires) ? ";expires=" + expires_date.toGMTString() : "") + 
		       ( (path) ? ";path=" + path : "") + 
		       ( (domain) ? ";domain=" + domain : "") + 
		       ( (secure) ? ";secure" : ""); 
		    document.cookie = cookieString; 
		} 
		/*
		End downloaded cookie functions
		*/
		
		function expandAll()
		{
			var treeObj = document.getElementById('dhtmlgoodies_tree');
			var images = treeObj.getElementsByTagName('IMG');
			for(var no=0;no<images.length;no++){
				if(images[no].className=='tree_plusminus' && images[no].src.indexOf(plusNode)>=0)expandNode(false,images[no]);
			}
		}
		function collapseAll()
		{
			var treeObj = document.getElementById('dhtmlgoodies_tree');
			var images = treeObj.getElementsByTagName('IMG');
			for(var no=0;no<images.length;no++){
				if(images[no].className=='tree_plusminus' && images[no].src.indexOf(minusNode)>=0)expandNode(false,images[no]);
			}
		}
		
		
		function expandNode(e,inputNode)
		{
			if(initExpandedNodes.length==0)initExpandedNodes=",";
			if(!inputNode)inputNode = this; 
			if(inputNode.tagName.toLowerCase()!='img')inputNode = inputNode.parentNode.getElementsByTagName('IMG')[0];	
			
			var inputId = inputNode.id.replace(/[^\d]/g,'');			
			
			var parentUl = inputNode.parentNode;
			var subUl = parentUl.getElementsByTagName('UL');

			if(subUl.length==0)return;
			if(subUl[0].style.display=='' || subUl[0].style.display=='none'){
				subUl[0].style.display = 'block';
				inputNode.src = minusNode;
				initExpandedNodes = initExpandedNodes.replace(',' + inputId+',',',');
				initExpandedNodes = initExpandedNodes + inputId + ',';
				
			}else{
				subUl[0].style.display = '';
				inputNode.src = plusNode;	
				initExpandedNodes = initExpandedNodes.replace(','+inputId+',',',');			
			}
			Set_Cookie(nameOfCookie,initExpandedNodes,60);
			
			
			
		}
		
		function initTree()
		{
			// Assigning mouse events
			var parentNode = document.getElementById('dhtmlgoodies_tree');
			var lis = parentNode.getElementsByTagName('LI'); // Get reference to all the images in the tree
			for(var no=0;no<lis.length;no++){
				var subNodes = lis[no].getElementsByTagName('UL');
				if(subNodes.length>0){
					lis[no].childNodes[0].style.visibility='visible';	
				}else{
					lis[no].childNodes[0].style.visibility='hidden';
				}
			}	
			
			var images = parentNode.getElementsByTagName('IMG');
			for(var no=0;no<images.length;no++){
				if(images[no].className=='tree_plusminus')images[no].onclick = expandNode;				
			}	

			var aTags = parentNode.getElementsByTagName('A');
			var cursor = 'pointer';
			if(document.all)cursor = 'hand';
			for(var no=0;no<aTags.length;no++){
				aTags[no].onclick = expandNode;		
				aTags[no].style.cursor = cursor;		
			}
			var initExpandedArray = initExpandedNodes.split(',');

			for(var no=0;no<initExpandedArray.length;no++){
				if(document.getElementById('plusMinus' + initExpandedArray[no])){
					var obj = document.getElementById('plusMinus' + initExpandedArray[no]);	
					expandNode(false,obj);
				}
			}				
		}
		//alert('cool');
		//window.onload = initTree;
