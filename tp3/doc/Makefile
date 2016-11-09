targets = informe.pdf

includes = secciones/exacto.tex secciones/experimentos.tex secciones/greedy.tex \
		   secciones/intro.tex secciones/src.tex

libs = caratula.sty

all: $(targets)

$(targets) : %.pdf : %.tex $(libs) $(includes)
	pdflatex $*
	pdflatex $*

clean:
	@rm $(targets) 2>/dev/null || true
	@rm ./*.aux 2>/dev/null || true
	@rm ./*.log 2>/dev/null || true
	@rm ./*.out 2>/dev/null || true
	@rm ./*.toc 2>/dev/null || true
	@rm ./*.fls 2>/dev/null || true
	@rm ./*.fdb_latexmk 2>/dev/null || true

