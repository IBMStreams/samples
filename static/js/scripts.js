$( "#filterDropdown" ).hide();

function handler1() {
  $("#filterButton").addClass('active');

  $(this).one("click", handler2);
  $( "#filterDropdown" ).show();

}

function handler2() {
  $("#filterButton").removeClass('active');

  $(this).one("click", handler1);
  $( "#filterDropdown" ).hide();
}

$("#filterButton").one("click", handler1);
$("#backtotop").click(function(){
  $("#searchTextF").focus();
});
