var paperfolds = [];          
var hiddenElements = $('.hidden');
var transEndEventNames = {
    'WebkitTransition' : 'webkitTransitionEnd',
    'MozTransition'    : 'transitionend',
    'OTransition'      : 'oTransitionEnd',
    'msTransition'     : 'MSTransitionEnd',
    'transition'       : 'transitionend'
},
transEndEventName = transEndEventNames[ Modernizr.prefixed('transition') ];

var paperfold = {
    percentage: 0,
    timeVirgin: true,
    init: function(element, maxHeight, toggleCallback){
      this.element = $(element);
      this.maxHeight = maxHeight;
      this.toggleCallback = toggleCallback;
      
      // get real element height
      this.height = this.element.css('height', 'auto').height();
      this.element.css('height', '');
      
      // calculate amount and height of the folds
      this.foldCount = Math.ceil(this.height / this.maxHeight);
      this.foldHeight = Math.floor(this.height / this.foldCount);
      
      // detach the elements children from the dom and cache them 
      this.content = this.element.children().detach();
      
      // add folds containing the previously cached children elements
      // to the element
      for(var i=0, j=0; i<this.foldCount; i++, j+=2){
        var topHeight = bottomHeight = Math.floor(this.foldHeight/2);
        if( (i+1) === this.foldCount && this.foldHeight/2 % 2 ) bottomHeight = this.height-(j+1)*topHeight;
        this.element.append( this.createFold(j, topHeight, bottomHeight) );
      }
      
      // cache the folds -> can i do this while creating them?
      // i mean i can of course cache them but then the dom connection is not there
      // i'd love to get a hint: @mrflix or mrflix@gmail.com
      this.folds = this.element.find('> .fold');
      this.bottoms = this.folds.find('> .bottom');
      this.tops = this.folds.find('> .top');
      
      // bind buttons
      this.element.next('.seeMore').click( $.proxy(this, 'toggle') );
      $('#go').click( $.proxy(this, 'toggle') );
      
      this.element.addClass('ready');
    },
    update: function(maxHeight){
      this.element.children().detach();
      this.element.html(this.content);
      this.init(this.element, maxHeight);
      if(this.percentage !== 0){
        this.open(this.percentage);
      }
    },
    createFold: function(j, topHeight, bottomHeight){
      var offsetTop = -j*topHeight;
      var offsetBottom = -this.height+j*topHeight+this.foldHeight;
      return $('<div>').addClass('fold').append(
              $('<div>').addClass('top').css('height', topHeight).append(
                $('<div>').addClass('inner').css('top', offsetTop).append(this.content.clone())
              ).add($('<div>').addClass('bottom').css('height', bottomHeight).append(
                $('<div>').addClass('inner').css('bottom', offsetBottom).append(this.content.clone())
              ))
            );
    },
    toggle: function(){
      this.element.toggleClass('visible');
      if(this.element.hasClass('visible')){
        // open
        // animate folds height (css transition)
        this.folds.height(this.foldHeight);
        
        // if the time slider was already used, reset the folds
        if(!this.timeVirigin) this.open(1);  
        
        // change button text
        $('#go').text('Close');
        
        // adapt time slider
        $(clock).animate({ time : 100 }, { duration : 400, step : function(now){
          $('#time').val(now);
        }});
      } else {
        // close
        // animate folds height (css transition)
        this.folds.height(0);
        
        // if the time slider was already used, reset the folds
        if(!this.timeVirigin) this.open(0);
        
        // change button text
        $('#go').text('Open');
        
        // adapt time slider
        $(clock).animate({ time : 0 }, { duration : 400, step : function(now){
          $('#time').val(now);
        }});
      }  
      this.tops.add(this.bottoms).css('background-color', '').css(transformString, '');
    },
    open: function(percentage){
      // cache percentage
      this.percentage = percentage;
      //clock.time = percentage * 100;
      timeVirigin = false;
      
      // change angle of tops and bottoms
      var c = this.foldHeight * percentage
      ,   a = b = this.foldHeight/2
      ,   part = 2*b*c
      ,   bottomAngle = part <= 0 ? 90 : Math.acos( (b*b+c*c-a*a) / part )*180/Math.PI
      ,   topAngle = 360-bottomAngle;
      
      this.tops.css(transformString, 'rotateX(' + topAngle + 'deg)');
      this.bottoms.css(transformString, 'rotateX(' + bottomAngle + 'deg)');
      
      // change folds height
      var foldHeight = this.height/this.foldCount * percentage;
      this.folds.height(foldHeight);
      
      // change the background color
      // from dark hsl(192,6,33) at 0
      // to light hsl(192,0,100) at 100
      var saturation = Math.round(6 - 6 * percentage)
      ,   lightness = 33 + Math.round(67 * percentage)
      ,   backgroundColor = 'hsl(192,'+saturation+'%,'+lightness+'%)';
      
      this.tops.add(this.bottoms).css('background-color', backgroundColor);
    },
};

$.each(hiddenElements, function(i, element){
    paperfolds[i] = Object.create(paperfold);
    paperfolds[i].init(element, 200);
});