import customtkinter as ctk
import os
os.system ('cls')
#region Funções da tela de Login
def validarLogineSenha(campoLogin, campoSenha, janelaLogin):
    loginCorreto = 'aluno'
    senhaCorreta = '1234'
    mensagem = 'Login ou senha incorretos, tente novamente.'
    if campoLogin.get() != loginCorreto.lower() or campoSenha.get() != senhaCorreta.lower():
        ctk.CTkLabel(janelaLogin,
                     text = mensagem,
                     text_color = 'red',
                     font = ('verdana', 12)).pack()
        return False
    else:
        return True

def entrar(janelaLogin, campoLogin, campoSenha):
    global janelaPrincipal
    if validarLogineSenha(campoLogin, campoSenha, janelaLogin):
        janelaLogin.destroy()

        janelaPrincipal = ctk.CTk()
        janelaPrincipal.geometry('600x350')
        janelaPrincipal.resizable(False, False)
        janelaPrincipal.title('Portal do Aluno')

        global notaPrimeiraUnidade, notaSegundaUnidade, notaTerceiraUnidade, resultado

        ctk.CTkLabel(janelaPrincipal,
             text = 'Sistema de Notas',
             text_color = 'black',
             font = ('Helvetica', 36, 'bold'),
             ).pack(pady = 10, padx = 20)
        
        notaPrimeiraUnidade = ctk.CTkEntry(janelaPrincipal,
                        width = 400,
                        height = 40,
                        border_color = 'grey',
                        placeholder_text = 'Digite aqui a sua nota da primeira unidade')
        notaPrimeiraUnidade.pack(pady = 10)

        notaSegundaUnidade = ctk.CTkEntry(janelaPrincipal,
                                width = 400,
                                height = 40,
                                border_color = 'grey',
                                placeholder_text = 'Digite aqui a sua nota da segunda unidade')
        notaSegundaUnidade.pack(pady = 10)

        notaTerceiraUnidade = ctk.CTkEntry(janelaPrincipal,
                                width = 400,
                                height = 40,
                                border_color = 'grey',
                                placeholder_text = 'Digite aqui a sua nota da terceira unidade')
        notaTerceiraUnidade.pack(pady = 10)

        botaoLimpar = ctk.CTkButton(janelaPrincipal,
                                    text = 'Limpar',
                                    text_color = 'White',
                                    fg_color = 'red',
                                    hover_color = 'dark red',
                                    width = 120,
                                    height = 40,
                                    cursor = 'hand2',
                                    font = ('arial', 12, 'bold'),
                                    command = limpar)
        botaoLimpar.place(x = 100, y = 255)

        botaoCalcular = ctk.CTkButton(janelaPrincipal,
                                    text = 'Calcular',
                                    text_color = 'White',
                                    fg_color = 'green',
                                    hover_color = 'dark green',
                                    width = 120,
                                    height = 40,
                                    cursor = 'hand2',
                                    font = ('arial', 12, 'bold'),
                                    command = calcularResultado)
        botaoCalcular.place(x = 370, y = 255)
        janelaPrincipal.bind('<Return>', lambda event: calcularResultado())

        resultado = ctk.CTkLabel(janelaPrincipal,
                                text = '',
                                text_color = 'black',
                                font = ('Arial', 13, 'bold')
                                )
        resultado.place(x = 160, y = 310)

        janelaPrincipal.mainloop()
#endregion

#region Funções da Tela Principal
def calcularResultado():
    unidade1 = float(notaPrimeiraUnidade.get())
    unidade2 = float(notaSegundaUnidade.get())
    unidade3 = float(notaTerceiraUnidade.get())

    media = (unidade1 + unidade2 + unidade3) / 3

    if media >= 7:
        situacao = 'Aprovado'
        
    elif media >=5 and media < 7:
        situacao = 'Recuperação'
        
    else:
        situacao = 'Reprovado'
    
    resultado.configure(text = f'A sua média é {media}. Sua situação é: {situacao}')
    

def limpar():
    notaPrimeiraUnidade.delete(0, ctk.END)
    notaSegundaUnidade.delete(0, ctk.END)
    notaTerceiraUnidade.delete(0, ctk.END)
    resultado.configure(text = '')
#endregion

#region Tela de Login
janelaLogin = ctk.CTk()
janelaLogin.geometry('350x300')
janelaLogin.resizable(False, False)
janelaLogin.title('Portal do Aluno')

ctk.set_appearance_mode('light')
ctk.set_default_color_theme("blue")

ctk.CTkLabel(janelaLogin,
             text = 'Login',
             text_color = 'black',
             font = ('Helvetica', 36, 'bold'),
             ).pack(pady = 10, padx = 20)

campoLogin = ctk.CTkEntry(janelaLogin,
                          width = 150,
                          height = 40,
                          border_color = 'grey',
                          placeholder_text = 'Login')
campoLogin.pack(pady = 10)

campoSenha = ctk.CTkEntry(janelaLogin,
                          width = 150,
                          height = 40,
                          border_color = 'grey',
                          placeholder_text = 'Senha',
                          show = '*')
campoSenha.pack(pady = 10)

botaoEntrar = ctk.CTkButton(janelaLogin,
                            text = 'Entrar',
                            text_color = 'White',
                            fg_color = 'blue',
                            hover_color = 'dark blue',
                            width = 150,
                            height = 40,
                            cursor = 'hand2',
                            font = ('arial', 12, 'bold'),
                            command = lambda: entrar(janelaLogin, campoLogin, campoSenha)
                            )
botaoEntrar.pack(pady = 10)
#bind para entrar com enter
janelaLogin.bind('<Return>', lambda event: entrar(janelaLogin, campoLogin, campoSenha))

janelaLogin.mainloop()
#endregion