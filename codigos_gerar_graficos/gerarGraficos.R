library("RSNNS")
library("corrgram")
library("ggplot2")
library("gridExtra")
library("gtable")
library(ggExtra)
#limpar tela
rm(list=ls())
cat('\014')
if(length(dev.list()) != 0){
  dev.off()  
}

dados <- read.table(
  "output.csv",
  sep=",",
  header = T,
  colClasses=c("numeric","numeric", "character", "character")
)



auxsum <- which(dados$action =="sum")
auxvector <- which(dados$action =="vector")

ggplot(dados[auxsum,], aes(x=size, y=time)) + 
  geom_line(aes(y=time, col=type)) + 
  labs(title="Tamanho da matriz X Tempo gasto na soma", 
       caption="Fonte: Autores", 
       y="Tempo", 
       color=NULL) +  # title and caption  # change to monthly ticks and labels
  scale_color_manual(labels = c("CUDA", "serial"), 
                     values = c("parallel"="#00ba38", "serial"="#f8766d", "mpi"="#0000ff","cuda"="#ff00ff")) +  # line color
  theme(axis.text.x = element_text(angle = 90, vjust=0.5, size = 8),  # rotate x axis text
        panel.grid.minor = element_blank())+  # turn off minor grid
geom_smooth(aes(col=type), method="lm", se=F)


ggplot(dados[auxvector,], aes(x=size, y=time)) + 
  geom_line(aes(y=time, col=type)) + 
  labs(title="Tamanho da matriz X Tempo gasto na criação da matriz", 
       caption="Fonte: Autores", 
       y="Tempo", 
       color=NULL) +  # title and caption  # change to monthly ticks and labels
  scale_color_manual(labels = c("CUDA",  "serial"), 
                     values = c("parallel"="#00ba38", "serial"="#f8766d", "mpi"="#0000ff","cuda"="#ff00ff")) +  # line color
  theme(axis.text.x = element_text(angle = 90, vjust=0.5, size = 8),  # rotate x axis text
        panel.grid.minor = element_blank())+  # turn off minor grid
  geom_smooth(aes(col=type), method="lm", se=F)

