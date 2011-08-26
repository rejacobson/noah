(function($) {
    
  $.fn.component_editor = function(settings)
  {
    console.log( 'setting up component editor' );
    
    var canvas = $(this[0]);
    var width = canvas.width(), height = canvas.height();
    var center_x = width/2, center_y = height/2;
    var dragging_element = null;    

    console.log( canvas );
    console.log( width +":"+ height );
    console.log( center_x +":"+ center_y );

    // Prevent default dragging of the div; Firefox will try to drag the div.
    canvas.bind('dragstart', function(event) {
      event.preventDefault()
    });
  
    canvas.mousemove( function(e) {
      if (dragging_element) {
        var self = $(this), offset = self.offset(),
            mouseX = e.pageX - offset.left,
            mouseY = e.pageY - offset.top;

        var t = parseInt( dragging_element.css('top') ), 
            l = parseInt( dragging_element.css('left') ),
            new_w = mouseX - l,
            new_h = mouseY - t;

        dragging_element.css({width: new_w+'px', height: new_h+'px'});
      } 
    });
    
    canvas.mousedown( function(e) {
      var x = e.layerX, y = e.layerY, bb = bounding_box(x, y);
      dragging_element = bb;
      console.log( 'mousedown -- '+ x +":"+ y);
      console.log( bb );
    });

    canvas.mouseup( function(e) {
      var x = e.layerX, y = e.layerY;
      dragging_element = null;
      console.log( 'mouseup -- '+ x +":"+ y);
      console.log( e );
    });

    bounding_box = function(x, y) {
      var bb = $('<div>').attr({class: 'bounding_box'}).css({position:'absolute', width:0, height:0, top:y+'px', left:x+'px'});
      return bb.appendTo( canvas );
    }       
  }


  components = {
    position: {
      attributes: {
        position: 0
      }
    },
    physics: {
      attributes: {
        position: 0,
        velocity: 0  
      }
    },
    render: {
      attributes: {
        position: 0,
        velocity: 0,
        offset: {x:0, y:0},
        image: ''
      },
    },
    collision: {
      attributes: {
        position: 0,
        velocity: 0,
        bounding_box: {
          offset: {x:0, y:0},
          t:0, b:0, l:0, r:0,
          w:0, h:0
        },
        bounding_sphere: {
          offset: {x:0, y:0},
          radius: 0
        }
      }
    }
  }

})(jQuery);
