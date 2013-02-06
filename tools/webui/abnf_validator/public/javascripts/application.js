// Place your application-specific JavaScript functions and classes here
// This file is automatically included by javascript_include_tag :defaults

function slide_tab_text(tab_name){
	$("#tab_text").slideUp("slow", function(){
		$("#tab_text").html(tab_name).slideDown();
	});
}

//AGGIORNA LA LISTA DEI FILES
function update_file_list(){
	$.ajax({
		type: 'post',
		url: '/users/get_files_list/',
		data: ({library : $('#library_select').val()}),
		dataType: 'html',
		success: function(data) {
			$('#files_list').html(data);
		}
	});
}

function simulation(){
	$.ajax({
		type: 'post',
		url: '/abnf_files/simulation/',
		data: ({lines : $('#simulation').val()}),
		dataType: 'html',
		success: function(data) {
			$("#console-text").html(data)
			if(!$("#console").is(":visible")){
				$("#console").show( 'slide', {direction: 'down'} ); 
			}
		}
	});
}


function update_libraries_list(){
	$.ajax({
		type: 'post',
		url: '/users/get_libraries_list/',
		dataType: 'html',
		success: function(data) {
			$('#library_select_div').fadeOut("fast", function(){$(this).html(data).fadeIn("fast")});
		}
	});
}

function validate_abnf(){
	$.ajax({
		type: 'post',
		url: '/abnf_files/validate/',
		data: ({content : $('#editor_textarea').val()}),
		dataType: 'html',
		success: function(data) {
			var line = data.match(/\d+/);
			countlines("editor_textarea", line);
			$('#validator_report').fadeOut("fast", function(){$(this).html(data).fadeIn("fast")});
		}
	});
}

function export_abnf(file_name){
	$.ajax({
		type: 'post',
		url: '/abnf_files/export/',
		data: ({content : $('#editor_textarea').val(), file_name : file_name}),
		dataType: 'html'
	});
}

function notify(){
	$.ajax({
		type: 'post',
		url: '/abnf_files/notify/',
		dataType: 'html',
		success: function(data) {
			$("#flash_messages").html(data).slideDown("slow");
			setTimeout(function(){
				$("#flash_messages").slideUp("slow");
			}, 3000);
		}
	});
}

//FUNZIONE CHE MODIFICA L'ALTEZZA DEL CORPO PER RENDERE L'APPLICATIVO ALTO ESATTAMENTE COME LA PAGINA.
//CHIAMATO SULL'ONRESIZE DEL BODY
function resize(){
	height=$(window).height()-181;
	$("#corpo").height(height);
}

//FUNZIONE CHE SETTA IL NUMERO DELLE LINEE DELLA TEXTAREA, CHIAMATA SULL'ONRESIZE DEL BODY
//E SULL'APERTURA DELL'EDITING DI UN FILE
function setRowNumber(){
	var tarea = document.getElementsByTagName('textarea')[0];
	var tarea2 = document.getElementById('editor_textarea');
	if(tarea){
		var height=$(window).height()-280;
		var width=$(window).width()/100*74;
		var cols=width/7.8;
		var rows = height / 15.2;
		tarea.rows = rows+1;
		tarea2.rows = rows;
		tarea.cols = cols;
		populate();
	}
}

//FUNZIONE CHE CREA L'ACCORDION TRA LE VOCI DEL MENÙ DELL'AMMINISTRATORE, PRENDE COME PARAMETRO L'ID DEL DIV DA VISUALIZZARE
var link_disabled = false;
function my_accordion(div_id, element){
	if(link_disabled == false){
		link_disabled = true;
		$(".accordion").each(function(index){
			if(this.id !== div_id && $(this).is(":visible")){
				$(".accordion").hide("slow");	
			}
		});
		$("#"+div_id).show("slow", function(){
			link_disabled = false;
			}
		);
	}
}

function loading(){
	$("#wsdl_file").html("<img src='images/loading.gif'>");
}

//FUNZIONE CHE COLORA I TAB ATTIVI, per ora il problema è che quando cambi layout ovviamente non funziona, va messo qualcosa in sessione
function set_active_class(){
	$('li').each(function(){
  	$(this).class("active_tab");
 	});
};

