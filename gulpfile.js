'use strict';

const gulp = require('gulp');

const less = require('gulp-less');
const watchLess = require('gulp-watch-less');
const babel = require('gulp-babel');
const rename = require('gulp-rename');
const concat = require('gulp-concat');
const LessPluginCleanCSS = require('less-plugin-clean-css');
const LessPluginAutoPrefix = require('less-plugin-autoprefix');
const cleanCSS = new LessPluginCleanCSS({advanced: true});
const autoPrefix = new LessPluginAutoPrefix({browsers: ['> 5%', 'not ie <= 9']});




const browserify  = require('browserify');
const babelify    = require('babelify');
const source      = require('vinyl-source-stream');
const buffer      = require('vinyl-buffer');
const uglify      = require('gulp-uglify');
const sourcemaps  = require('gulp-sourcemaps');
const livereload  = require('gulp-livereload');

const es = require('event-stream');
const gutil = require('gulp-util');




const paths = {
  less: ['./sites/all/modules/gridstack_field/less/*.less'],
  es6: ['./sites/all/modules/gridstack_field/es6/*.es6'],
  js: './sites/all/modules/gridstack_field/js'
};
const jsFiles = [
  './sites/all/modules/gridstack_field/es6/gridstack_field.es6',
  './sites/all/modules/gridstack_field/es6/gridstack_field_view.es6'
];

// Process LESS files from main theme and custom modules.
const processLess = function (pipe, env) {
  var options = {};
  if (env != 'dev') {
    options = {plugins: [autoPrefix, cleanCSS]};  }
    return pipe
      .pipe(less(options))
      .pipe(rename({suffix: '.min'}))
      .pipe(gulp.dest('./sites/all/modules/gridstack_field/css'));
};

// One-time processing of files.
gulp.task('less', function () {
  return processLess(gulp.src(paths.less));
});

// Process ECMAScript6 files.
// gulp.task('es6', function () {
//   return gulp.src(paths.es6)
//     .pipe(babel())
//     .pipe(rename({suffix: '-compiled'}))
//     .pipe(gulp.dest(function (file) {
//       return file.base;
//     }));
// });
// gulp.task('es6', function () {
//   // app.js is your main JS file with all your module inclusions
//   return browserify({entries: jsFiles, debug: true})
//     .transform("babelify", { presets: ["es2015"] })
//     .bundle()
//     .pipe(source('gridstack_field.min.js'))
//     .pipe(buffer())
//     // .pipe(sourcemaps.init())
//     .pipe(uglify())
//     // .pipe(sourcemaps.write('./map'))
//     .pipe(gulp.dest(paths.js));
// });

gulp.task('es6', function() {
  // map them to our stream function
  let tasks = jsFiles.map(function(entry) {
    return browserify({ entries: [entry] })
      .transform("babelify", { presets: ["es2015"] })
      .bundle()
      .pipe(source(entry))
      .pipe(buffer())
      .pipe(sourcemaps.init())
      .pipe(uglify())
      .on('error', function (err) { gutil.log(gutil.colors.red('[Error]'), err.toString()); })
      // rename them to have "bundle as postfix"
      .pipe(rename({
        dirname: './',
        extname: '.min.js'
      }))
      .pipe(sourcemaps.write('./map'))
      .pipe(gulp.dest(paths.js));
  });
  // create a merged stream
  return es.merge.apply(null, tasks);
});





gulp.task('build', ['less', 'es6']);
gulp.task('default', ['build', 'watch']);
