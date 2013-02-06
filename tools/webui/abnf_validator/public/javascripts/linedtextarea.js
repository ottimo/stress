//FUNCTION THAT SYNC THE SCROLL BETWEEN 2 TEXTAREA, THE LINED ONE AND THE EDITOR ONE
window.sync = function(e){
	var textarea = document.getElementById("lines");
	var source   = document.getElementById("editor_textarea");
	textarea.scrollTop = source.scrollTop;
}

//FUNCTION THAT FILL THE LINED TEXTAREA WITH LINES NUMBERS
window.populate = function populate(){
	var textarea = document.getElementById("lines");
	var str = '';
	for(var i=1;i < 99999;i++){
		str = str + (i +'\r\n');
	}
	textarea.value = str;
}

//FUNCTION THAT SELECT THE LINE WITH ERROR ON VALIDATION
function showWhereInTextarea(area, start, len){
	var textarea = document.getElementById(area);
	if(textarea.setSelectionRange){
		textarea.setSelectionRange(parseInt(start), (parseInt(start)+parseInt(len)));
		textarea.setSelectionRange(start,start+len);
    textarea.focus();
	}
	//else{
	//	var range = textarea.createTextRange();
	//	range.collapse(true);        
	//	range.moveStart('character',parseInt(start) );
	//	range.moveEnd('character',parseInt(len));
	//	range.select();
	//}	
}

//FUNCTION THAT COUNT LINES IN EDITOR TEXTAREA, TO SELECT THE FIRST WITH ERRORS
function countlines(area, line)  
{  
 // trim trailing return char if exists
 	var textarea = area;  
	var area = document.getElementById(area);
	var text = area.value.replace(/\s+$/g,"");  
	var split = text.split("\n");
	var start = 0;
	var len = 0;
	if(line == 1){
		start = 0;
		len = split[0].length;
	}else{
		for(var i = 0; i < line - 1; i++){
			start += split[i].length;
			start += 1;
			len = split[i+1].length;
		}
	}
	showWhereInTextarea(textarea, start, len)
}
